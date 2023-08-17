#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include "g03.h"
#include "estream.h"
#include "types.h"
#include "museum.h"
#include "extras.h"

#ifndef _WIN32
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#endif

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

/* if hampus ever sees this code hes divorcing me */

int main(int argc, char **argv) {
    const int width = 800;
    const int height = 600;

    /* i like to keep a 20px boundary between
     * ui elements and the edge of the screen */
    const int lXBnd = 20;
    const int rXBnd = width - 20;
    const int tYBnd = 20;
    const int bYBnd = height - 20;

    const int fontSize = 20;
    const int smallFontSize = 10;

    /* input-files */
    FilePathList droppedFiles;
    bool droppedFile = false;
    std::vector<std::string> droppedFilesVec = {""};

#ifndef _WIN32 // currently my savepath; speeds up testing
    std::ifstream cfgf("/home/lvlrk/.config/musave.json");
    if(cfgf.is_open()) {
        json cfg = json::parse(cfgf);
        droppedFilesVec[0] = cfg["SavePath"];
    }
#endif

    InitWindow(width, height, "musave");

    /* savefile io variables */
    estream in;
    in.endian() = std::endian::big;
    std::stringstream data;
    std::vector<char> dataVec(museum::SAVE_SIZE - 4);

    /* g01 / galaga remix variables */
    std::vector<std::string> g01worlds = {"1", "2", "3"};
    int g01worldIdx = 0;
    int g01stageIdx = 0;
    std::vector<std::string> sections = {"Galaga Remix", "Rally-X Remix", "Pac 'N Roll Remix", "Gator-Panic Remix", "Pac Motos"};
    int sectionIdx = 0;
    std::vector<int> g01EnemiesCount = {139, 224, 278,
                                        286, 366, 193,
                                        320, 274, 118};
    bool g01complete;

    /* g03 / rally-x remix variables */
    std::vector<std::string> g02worlds = {"1", "2", "3", "4"};
    int g02worldIdx = 0;
    int g02stageIdx = 0;

    /* g03 / pac n roll remix variables */
    std::vector<std::string> g03worlds = {"1", "2", "3", "4", "5"};
    int g03worldIdx = 0;
    int g03stageIdx = 0;
    bool g03complete;

    // TODO: g04 / gator panic support

    /* g07 / pac motos variables */
    std::vector<std::string> g07worlds = {"1", "2", "3", "4", "5"};
    int g07worldIdx = 0;
    int g07stageIdx = 0;
    bool g07complete;
    int g07lives;
    int g07power;
    int g07jump;
    int g07charge;

    /* input variables */
    std::string input = "";
    std::string lastInput = "";
    bool doInput = false;
    bool doInitLoad = false;

    /* raw save structures */
    std::vector<struct museum::GalagaSave> galagaSaves(9);
    std::vector<struct museum::RallyXSave> rallyXSaves(25);
    std::vector<struct museum::PacSave> pacSaves(26);
    std::vector<struct museum::MotosSave> motosSaves(35);

    if(droppedFilesVec[0] != "") {
        Load(in, droppedFilesVec, data, galagaSaves, g01complete, rallyXSaves, pacSaves, g03complete, motosSaves, g07complete, g07lives, g07power, g07jump, g07charge);
        doInitLoad = true;
    }

    while(!WindowShouldClose()) {
        /* --- logic section --- */
        /* input logic */
        if(doInput) {
            int key = GetKeyPressed();
            char ch = GetCharPressed();

            if(key != 0) {
                input += ch;

                if(key == KEY_ENTER) {
                    lastInput = input;
                    doInput = false;
                    input = "";
                } else if(key == KEY_BACKSPACE || key == KEY_DELETE) {
                    input.pop_back();
                } else if(key == KEY_END || key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT) {
                    doInput = false;
                    input = "";
                }
            }
        }

        if(!droppedFile) {
            droppedFiles = LoadDroppedFiles();
            if(droppedFiles.count == 1) {
                droppedFile = true;
                droppedFilesVec[0] = droppedFiles.paths[0];
            }
            UnloadDroppedFiles(droppedFiles);
        }

        if(!doInitLoad) {
            if(std::string(GetFileName(droppedFilesVec[0].c_str())) == "museum.bin") {
                doInitLoad = true;
                Load(in, droppedFilesVec, data, galagaSaves, g01complete, rallyXSaves, pacSaves, g03complete, motosSaves, g07complete, g07lives, g07power, g07jump, g07charge);
            }
        }

        /* --- draw section --- */
        BeginDrawing();
        ClearBackground(BLACK);

        /* input prompt */
        if(doInput) {
            DrawText("INPUT: ", lXBnd, bYBnd, fontSize, WHITE);

            DrawText(TextFormat("%s", input.c_str()), lXBnd + MeasureText("INPUT: ", fontSize), bYBnd, fontSize, WHITE);
        }

        if(lastInput != "") {
            DrawText(TextFormat("CLIPBOARD: %s", lastInput.c_str()), lXBnd, bYBnd - fontSize, fontSize, WHITE);
        }

        /* g01 - g07 buttons */
        for(int i = 0; i < sections.size(); i++) {
            if(i == sectionIdx) {
                if(GuiButton({(float)(lXBnd + (i * 100)), fontSize, 100, 40 + 30}, sections[i].c_str())) {
                    sectionIdx = i;
                }
            } else {
                if(GuiButton({(float)(lXBnd + (i * 100)), fontSize, 100, 40}, sections[i].c_str())) {
                    sectionIdx = i;
                }
            }
        }

        if(doInitLoad) {
            if(GuiButton({rXBnd - 200 - fontSize, fontSize, 100, fontSize}, "READ")) {
                Load(in, droppedFilesVec, data, galagaSaves, g01complete, rallyXSaves, pacSaves, g03complete, motosSaves, g07complete, g07lives, g07power, g07jump, g07charge);
            }
            if(GuiButton({rXBnd - 100, fontSize * 3, 100, fontSize}, "RESET")) {
                std::ofstream out(droppedFilesVec[0]);
                for(int i = 0; i < museum::SAVE_SIZE; i++) out.put(0);
                out.close();

                Load(in, droppedFilesVec, data, galagaSaves, g01complete, rallyXSaves, pacSaves, g03complete, motosSaves, g07complete, g07lives, g07power, g07jump, g07charge);
            }
            if(GuiButton({rXBnd - 100, fontSize, 100, fontSize}, "WRITE")) {
                in.open(droppedFilesVec[0]);
                in.seekp(museum::GALAGA_OFF, in.beg);

                for(int i = 0; i < 9; i++) {
                    in.write_int(galagaSaves[i].shot);
                    in.write_int(galagaSaves[i].unused1);
                    in.write_int(galagaSaves[i].accuracy);
                    in.write_int(galagaSaves[i].hp);
                    in.write_int(galagaSaves[i].score);
                    in.write_int(galagaSaves[i].shotBadge);
                    in.write_int(galagaSaves[i].accuracyBadge);
                    in.write_int(galagaSaves[i].scoreBadge);
                    in.write_int(galagaSaves[i].hpBadge);
                    in.write_int(galagaSaves[i].flag);
                    in.write_int(galagaSaves[i].unused2);
                }

                in.seekp(museum::GALAGA_COMPLETE_OFF + 4);
                u32 tmp1;
                if(g01complete) {
                    tmp1 = 1;
                } else {
                    tmp1 = 0;
                }

                in.endian() = std::endian::little;
                in.write_int(tmp1);
                in.endian() = std::endian::big;

                in.seekg(museum::RALLYX_OFF, in.beg);
                for(int i = 0; i < 25; i++) {
                    in.write_int(rallyXSaves[i].time);
                    in.write_int(rallyXSaves[i].score);
                    in.write_int(rallyXSaves[i].scoreBadge);
                    in.write_int(rallyXSaves[i].timeBadge);
                    in.write_int(rallyXSaves[i].flag);
                }

                if(g03complete) {
                    pacSaves[25].flag = 1;
                } else pacSaves[25].flag = 0;

                in.seekp(museum::PAC_OFF, in.beg);
                for(int i = 0; i < 26; i++) {
                    in.write_int(pacSaves[i].time);
                    in.write_int(pacSaves[i].unused1);
                    in.write_int(pacSaves[i].collected);
                    in.write_int(pacSaves[i].max);
                    in.write_int(pacSaves[i].unused2);
                    in.write_int(pacSaves[i].timeBadge);
                    in.write_int(pacSaves[i].ghostBadge);
                    in.write_int(pacSaves[i].collectedBadge);
                    in.write_int(pacSaves[i].flag);
                }

                in.seekp(museum::MOTOS_OFF, in.beg);
                for(int i = 0; i < 35; i++) {
                    in.write_int(motosSaves[i].score);
                    in.write_int(motosSaves[i].time);
                    in.write_int(motosSaves[i].scoreBadge);
                    in.write_int(motosSaves[i].timeBadge);
                    in.write_int(motosSaves[i].unused);
                    in.write_int(motosSaves[i].flag);
                }

                in.seekp(museum::MOTOS_LIVES_OFF, in.beg);
                in.write_int(g07lives);
                in.seekp(museum::MOTOS_POWER_OFF, in.beg);
                in.write_int(g07power);
                in.write_int(g07jump);
                in.write_int(g07charge);


                std::vector<char> tmp(museum::SAVE_SIZE - 4);
                in.seekg(4, in.beg);
                in.read(reinterpret_cast<char*>(&tmp[0]), tmp.size());

                u32 tmpCrc = crc32(0L, Z_NULL, 0);
                for(int i = 0; i < tmp.size(); i++) {
                    tmpCrc = crc32(tmpCrc, reinterpret_cast<u8*>(&tmp[0]) + i, 1);
                }

                in.seekp(0, in.beg);

                tmpCrc = std::byteswap(tmpCrc);

                in.write(reinterpret_cast<char*>(&tmpCrc), 4);
                
                in.close();
            }

            int tmpX = width - 40;

            if(sectionIdx == 3) {
                DrawText("sorry bro Gator-Panic isn't supported yet", 200, 300, fontSize, WHITE);
            }

            if(sectionIdx == 4) {
                if(GuiButton({lXBnd, 105, 100, 20}, TextFormat("WORLD %d >>>", g07worldIdx + 1))) {
                    if(g07worldIdx + 1 > g07worlds.size() - 1) g07worldIdx = 0;
                    else g07worldIdx++;

                    g07stageIdx = 0;
                }

                if(GuiLabelButton({140, 105, 60, 10}, TextFormat("LIVES: %d", g07lives))) {
                    doInput = true;

                    if(lastInput != "") {
                        g07lives = std::stoi(lastInput);
                    }
                }
                if(GuiLabelButton({190, 95, 60, 10}, TextFormat("POWER: %d", g07power))) {
                    doInput = true;

                    if(lastInput != "") {
                        g07power = std::stoi(lastInput);
                    }
                }
                if(GuiLabelButton({190, 105, 60, 10}, TextFormat("JUMP: %d", g07jump))) {
                    doInput = true;

                    if(lastInput != "") {
                        g07jump = std::stoi(lastInput);
                    }
                }
                if(GuiLabelButton({190, 115, 60, 10}, TextFormat("CHARGE: %d", g07charge))) {
                    doInput = true;

                    if(lastInput != "") {
                        g07charge = std::stoi(lastInput);
                    }
                }

                for(int i = 0; i < 7; i++) {
                    if(i == 6) GuiGroupBox({20 + (float)(((int)(tmpX / 7)) * i), 140, (float)((int)(tmpX / 7)), (height - 160) - 40}, TextFormat("%d - BOSS", g07worldIdx + 1));
                    else GuiGroupBox({20 + (float)(((int)(tmpX / 7)) * i), 140, (float)((int)(tmpX / 7)), (height - 160) - 40}, TextFormat("%d - %d", g07worldIdx + 1, i + 1));
                }

                for(int i = 0; i < 7; i++) {
                    int g07stageVecIdx = g07StageIdx(g07worldIdx, i);

                    if(GuiLabelButton({40 + (float)((int)(tmpX / 7)) * i, 160, 50, 20}, TextFormat("High Score: %d", motosSaves[g07stageVecIdx].score))) {
                        doInput = true;

                        if(lastInput != "") {
                            motosSaves[g07stageVecIdx].score = std::stoi(lastInput);
                        }
                    }
                    if(motosSaves[g07stageVecIdx].scoreBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 7)) * i, 180, 50, 20}, "PERFECT", GOLD)) {
                            motosSaves[g07stageVecIdx].scoreBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 7)) * i, 180, 50, 20}, "PERFECT", GRAY)) {
                            motosSaves[g07stageVecIdx].scoreBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(tmpX / 7)) * i, 200, 50, 20}, TextFormat("Time: %d", motosSaves[g07stageVecIdx].time))) {
                        doInput = true;

                        if(lastInput != "") {
                            motosSaves[g07stageVecIdx].time = std::stoi(lastInput);
                        }

                    }
                    if(motosSaves[g07stageVecIdx].timeBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 7)) * i, 220, 50, 20}, "PERFECT", GOLD)) {
                            motosSaves[g07stageVecIdx].timeBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 7)) * i, 220, 50, 20}, "PERFECT", GRAY)) {
                            motosSaves[g07stageVecIdx].timeBadge = 1;
                        }
                    }

                    if(motosSaves[g07stageVecIdx].flag) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 7)) * i, 540, 50, 30}, "UNLOCKED", GOLD)) motosSaves[g07stageVecIdx].flag = 0;
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 7)) * i, 540, 50, 30}, "LOCKED", GRAY)) motosSaves[g07stageVecIdx].flag = 1;
                    }
                }

            }

            if(sectionIdx == 2) {
                if(GuiButton({20, 105, 100, 20}, TextFormat("WORLD %d >>>", g03worldIdx + 1))) {
                    if(g03worldIdx + 1 > g03worlds.size() - 1) g03worldIdx = 0;
                    else g03worldIdx++;

                    g03stageIdx = 0;
                }

                if(g03worldIdx == 0) {
                    for(int i = 0; i < 5; i++) {
                        GuiGroupBox({20 + (float)(((int)(tmpX / 5)) * i), 140, (float)((int)(tmpX / 5)), (height - 160) - 40}, TextFormat("%d - %d", g03worldIdx + 1, i + 1));
                    }
                } else if(g03worldIdx == 1) {
                    for(int i = 0; i < 4; i++) {
                        if(i == 3) GuiGroupBox({20 + (float)(((int)(tmpX / 4)) * i), 140, (float)((int)(tmpX / 4)), (height - 160) - 40}, TextFormat("%d - BOSS", g03worldIdx + 1));
                        else GuiGroupBox({20 + (float)(((int)(tmpX / 4)) * i), 140, (float)((int)(tmpX / 4)), (height - 160) - 40}, TextFormat("%d - %d", g03worldIdx + 1, i + 1));
                    }
                } else if(g03worldIdx == 2) {
                    for(int i = 0; i < 5; i++) {
                        if(i == 4) GuiGroupBox({20 + (float)(((int)(tmpX / 5)) * i), 140, (float)((int)(tmpX / 5)), (height - 160) - 40}, TextFormat("%d - BOSS", g03worldIdx + 1));
                        else GuiGroupBox({20 + (float)(((int)(tmpX / 5)) * i), 140, (float)((int)(tmpX / 5)), (height - 160) - 40}, TextFormat("%d - %d", g03worldIdx + 1, i + 1));
                    }
                } else if(g03worldIdx == 3) {
                    for(int i = 0; i < 6; i++) {
                        if(i == 5) GuiGroupBox({20 + (float)(((int)(tmpX / 6)) * i), 140, (float)((int)(tmpX / 6)), (height - 160) - 40}, TextFormat("%d - BOSS", g03worldIdx + 1));
                        else GuiGroupBox({20 + (float)(((int)(tmpX / 6)) * i), 140, (float)((int)(tmpX / 6)), (height - 160) - 40}, TextFormat("%d - %d", g03worldIdx + 1, i + 1));
                    }
                } else if(g03worldIdx == 4) {
                    for(int i = 0; i < 5; i++) {
                        if(i == 4) GuiGroupBox({20 + (float)(((int)(tmpX / 5)) * i), 140, (float)((int)(tmpX / 5)), (height - 160) - 40}, TextFormat("%d - BOSS", g03worldIdx + 1));
                        else GuiGroupBox({20 + (float)(((int)(tmpX / 5)) * i), 140, (float)((int)(tmpX / 5)), (height - 160) - 40}, TextFormat("%d - %d", g03worldIdx + 1, i + 1));
                    }
                }

                int div;
                if(g03worldIdx == 0) div = 5;
                if(g03worldIdx == 1) div = 4;
                if(g03worldIdx == 2) div = 5;
                if(g03worldIdx == 3) div = 6;
                if(g03worldIdx == 4) div = 5;

                for(int i = 0; i < div; i++) {
                    int g03stageVecIdx = g03StageIdx(g03worldIdx, i);

                    if(GuiLabelButton({40 + (float)((int)(tmpX / div)) * i, 160, 50, 20}, TextFormat("Pac-Dots: %d / %d", pacSaves[g03stageVecIdx].collected, pacSaves[g03stageVecIdx].max))) {
                        doInput = true;

                        if(lastInput != "") {
                            pacSaves[g03stageVecIdx].collected = std::stoi(lastInput);
                        }
                    }
                    if(pacSaves[g03stageVecIdx].collectedBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / div)) * i, 180, 50, 20}, "PERFECT", GOLD)) {
                            pacSaves[g03stageVecIdx].collectedBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / div)) * i, 180, 50, 20}, "PERFECT", GRAY)) {
                            pacSaves[g03stageVecIdx].collectedBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(tmpX / div)) * i, 200, 50, 20}, TextFormat("Time: %d", pacSaves[g03stageVecIdx].time))) {
                        doInput = true;

                        if(lastInput != "") {
                            pacSaves[g03stageVecIdx].time = std::stoi(lastInput);
                        }

                    }
                    if(pacSaves[g03stageVecIdx].timeBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / div)) * i, 220, 50, 20}, "PERFECT", GOLD)) {
                            pacSaves[g03stageVecIdx].timeBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / div)) * i, 220, 50, 20}, "PERFECT", GRAY)) {
                            pacSaves[g03stageVecIdx].timeBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(tmpX / div)) * i, 240, 50, 20}, TextFormat("Ghosts: %d", pacSaves[g03stageVecIdx].unused2))) {
                        doInput = true;

                        if(lastInput != "") {
                            pacSaves[g03stageVecIdx].unused2 = std::stoi(lastInput);
                        }

                    }
                    if(pacSaves[g03stageVecIdx].ghostBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / div)) * i, 260, 50, 20}, "PERFECT", GOLD)) {
                            pacSaves[g03stageVecIdx].ghostBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / div)) * i, 260, 50, 20}, "PERFECT", GRAY)) {
                            pacSaves[g03stageVecIdx].ghostBadge = 1;
                        }
                    }


                    if(pacSaves[g03stageVecIdx].flag) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / div)) * i, 540, 50, 30}, "UNLOCKED", GOLD)) pacSaves[g03stageVecIdx].flag = 0;
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / div)) * i, 540, 50, 30}, "UNLOCKED", GRAY)) pacSaves[g03stageVecIdx].flag = 1;
                    }

                    if(i == 4 && g03worldIdx == 4) {
                        if(g03complete) {
                            if(GuiLabelButtonColor({40 + (float)((int)(tmpX /5)) * i, height - 40, 100, 30}, "FINAL BOSS COMPLETE", GOLD)) {
                                g03complete = false;
                            }
                        } else {
                            if(GuiLabelButtonColor({40 + (float)((int)(tmpX /5)) * i, height - 40, 100, 30}, "FINAL BOSS COMPLETE", GRAY)) {
                                g03complete = true;
                            }
                        }
                    }

                }
            }

            if(sectionIdx == 1) {
                if(GuiButton({20, 105, 100, 20}, TextFormat("WORLD %d >>>", g02worldIdx + 1))) {
                    if(g02worldIdx + 1 > g02worlds.size() - 1) g02worldIdx = 0;
                    else g02worldIdx++;

                    g02stageIdx = 0;
                }

                for(int i = 0; i < 6; i++) {
                    if(i == 5) GuiGroupBox({20 + (float)(((int)(tmpX / 6)) * i), 140, (float)((int)(tmpX / 6)), (height - 160) - 40}, TextFormat("%d - REDMODE", g02worldIdx + 1));
                    else GuiGroupBox({20 + (float)(((int)(tmpX / 6)) * i), 140, (float)((int)(tmpX / 6)), (height - 160) - 40}, TextFormat("%d - %d", g02worldIdx + 1, i + 1));
                }

                for(int i = 0; i < 6; i++) {
                    int g02stageVecIdx = g02StageIdx(g02worldIdx, i);

                    if(GuiLabelButton({40 + (float)((int)(tmpX / 6)) * i, 160, 50, 20}, TextFormat("High Score: %d", rallyXSaves[g02stageVecIdx].score))) {
                        doInput = true;

                        if(lastInput != "") {
                            rallyXSaves[g02stageVecIdx].score = std::stoi(lastInput);
                        }
                    }
                    if(rallyXSaves[g02stageVecIdx].scoreBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 6)) * i, 180, 50, 20}, "PERFECT", GOLD)) {
                            rallyXSaves[g02stageVecIdx].scoreBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 6)) * i, 180, 50, 20}, "PERFECT", GRAY)) {
                            rallyXSaves[g02stageVecIdx].scoreBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(tmpX / 6)) * i, 200, 50, 20}, TextFormat("Time: %d", rallyXSaves[g02stageVecIdx].time))) {
                        doInput = true;

                        if(lastInput != "") {
                            rallyXSaves[g02stageVecIdx].time = std::stoi(lastInput);
                        }

                    }
                    if(rallyXSaves[g02stageVecIdx].timeBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 6)) * i, 220, 50, 20}, "PERFECT", GOLD)) {
                            rallyXSaves[g02stageVecIdx].timeBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 6)) * i, 220, 50, 20}, "PERFECT", GRAY)) {
                            rallyXSaves[g02stageVecIdx].timeBadge = 1;
                        }
                    }

                    if(rallyXSaves[g02stageVecIdx].flag) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 6)) * i, 540, 50, 30}, "COMPLETE", GOLD)) rallyXSaves[g02stageVecIdx].flag = 0;
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX / 6)) * i, 540, 50, 30}, "COMPLETE", GRAY)) rallyXSaves[g02stageVecIdx].flag = 1;
                    }
                }
            }

            if(sectionIdx == 0) {
                if(GuiButton({20, 105, 100, 20}, TextFormat("WORLD %d >>>", g01worldIdx + 1))) {
                    if(g01worldIdx + 1 > g01worlds.size() - 1) g01worldIdx = 0;
                    else g01worldIdx++;

                    g01stageIdx = 0;
                }

                /*
                if(GuiButton({120, 105, 100, 20}, TextFormat("STAGE %d", g01stageIdx + 1))) {
                    if(g01stageIdx + 1 > 2) {
                        g01stageIdx = 0;
                        if(g01worldIdx + 1 > g01worlds.size() - 1) {
                            g01worldIdx = 0;
                            g01stageIdx = 0;
                        } else g01worldIdx++;
                    }
                    else g01stageIdx++;
                }
                */

                for(int i = 0; i < 3; i++) {
                    GuiGroupBox({20 + (float)(((int)(tmpX / 3)) * i), 140, (float)((int)(tmpX / 3)), (height - 160) - 40}, TextFormat("%d - %d", g01worldIdx + 1, i + 1));
                }

                for(int i = 0; i < 3; i++) {
                    int g01stageVecIdx = g01StageIdx(g01worldIdx, i);

                    if(GuiLabelButton({40 + (float)((int)(tmpX /3)) * i, 160, 100, 20}, TextFormat("High Score: %d", galagaSaves[g01stageVecIdx].score))) {
                        doInput = true;

                        if(lastInput != "") {
                            galagaSaves[g01stageVecIdx].score = std::stoi(lastInput);
                        }
                    }
                    if(galagaSaves[g01stageVecIdx].scoreBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i + 170, 160, 50, 20}, "PERFECT", GOLD)) {
                            galagaSaves[g01stageVecIdx].scoreBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i + 170, 160, 50, 20}, "PERFECT", GRAY)) {
                            galagaSaves[g01stageVecIdx].scoreBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(tmpX /3)) * i, 180, 100, 20}, TextFormat("Enemies Shot Down: %d / %d", galagaSaves[g01stageVecIdx].shot, g01EnemiesCount[g01stageVecIdx]))) {
                        doInput = true;

                        if(lastInput != "") {
                            galagaSaves[g01stageVecIdx].shot = std::stoi(lastInput);
                        }

                    }
                    if(galagaSaves[g01stageVecIdx].shotBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i + 170, 180, 50, 20}, "PERFECT", GOLD)) {
                            galagaSaves[g01stageVecIdx].shotBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i + 170, 180, 50, 20}, "PERFECT", GRAY)) {
                            galagaSaves[g01stageVecIdx].shotBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(tmpX /3)) * i, 200, 100, 20}, TextFormat("Accuracy: %.1f%%", (float)(galagaSaves[g01stageVecIdx].accuracy) / 10))) {
                        doInput = true;

                        if(lastInput != "") {
                            galagaSaves[g01stageVecIdx].accuracy = std::stof(lastInput) * 10;
                        }

                    }
                    if(galagaSaves[g01stageVecIdx].accuracyBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i + 170, 200, 50, 20}, "PERFECT", GOLD)) {
                            galagaSaves[g01stageVecIdx].accuracyBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i + 170, 200, 50, 20}, "PERFECT", GRAY)) {
                            galagaSaves[g01stageVecIdx].accuracyBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(tmpX /3)) * i, 220, 100, 20}, TextFormat("Defense: %d / 7", galagaSaves[g01stageVecIdx].hp))) {
                        doInput = true;

                        if(lastInput != "") {
                            galagaSaves[g01stageVecIdx].hp = std::stoi(lastInput);
                        }

                    }
                    if(galagaSaves[g01stageVecIdx].hpBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i + 170, 220, 50, 20}, "PERFECT", GOLD)) {
                            galagaSaves[g01stageVecIdx].hpBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i + 170, 220, 50, 20}, "PERFECT", GRAY)) {
                            galagaSaves[g01stageVecIdx].hpBadge = 1;
                        }
                    }

                    if(galagaSaves[g01stageVecIdx].flag) {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i, 540, 200, 30}, "UNLOCKED", GOLD)) galagaSaves[g01stageVecIdx].flag = 0;
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i, 540, 200, 30}, "LOCKED", GRAY)) galagaSaves[g01stageVecIdx].flag = 1;
                    }

                    if(i == 2 && g01worldIdx == 2) {
                        if(g01complete) {
                            if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i, 560, 100, 30}, "BOSS COMPLETE", GOLD)) {
                                g01complete = false;
                            }
                        } else {
                            if(GuiLabelButtonColor({40 + (float)((int)(tmpX /3)) * i, 560, 100, 30}, "BOSS COMPLETE", GRAY)) {
                                g01complete = true;
                            }
                        }
                    }
                }
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
