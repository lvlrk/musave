#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unpac/g03.h>
#include <unpac/estream.h>
#include <unpac/types.h>
#include <unpac/museum.h>

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int g01StageIdx(int worldIdx, int sectionIdx) {
    int stageIdx = 0;

    if(worldIdx == 0) {
        if(sectionIdx == 0) {
            stageIdx = 0;
        } else if(sectionIdx == 1) {
            stageIdx = 1;
        } else if(sectionIdx == 2) {
            stageIdx = 2;
        }
    } else if(worldIdx == 1) {
        if(sectionIdx == 0) {
            stageIdx = 3;
        } else if(sectionIdx == 1) {
            stageIdx = 4;
        } else if(sectionIdx == 2) {
            stageIdx = 5;
        }
    } else if(worldIdx == 2) {
        if(sectionIdx == 0) {
            stageIdx = 6;
        } else if(sectionIdx == 1) {
            stageIdx = 7;
        } else if(sectionIdx == 2) {
            stageIdx = 8;
        }
    }

    return stageIdx;
}

void Load(estream& in, std::vector<std::string>& droppedFilesVec, std::stringstream& data, std::vector<struct museum::GalagaSave>& galagaSaves, bool& g01complete) {
    in.open(droppedFilesVec[0]);
    if(!in.is_open()) {
        std::cerr << droppedFilesVec[0] << " not opened!\n";
        return;
    };

    u32 crc = in.read_int<u32>();

    data << in.rdbuf();

    in.seekg(museum::GALAGA_OFF, in.beg);
    for(int i = 0; i < 10; i++) {
        galagaSaves[i].shot = in.read_int<u32>();
        galagaSaves[i].unused1 = in.read_int<u32>();
        galagaSaves[i].accuracy = in.read_int<u32>();
        galagaSaves[i].hp = in.read_int<u32>();
        galagaSaves[i].score = in.read_int<u32>();
        galagaSaves[i].shotBadge = in.read_int<u32>();
        galagaSaves[i].accuracyBadge = in.read_int<u32>();
        galagaSaves[i].scoreBadge = in.read_int<u32>();
        galagaSaves[i].hpBadge = in.read_int<u32>();
        galagaSaves[i].flag = in.read_int<u32>();
        galagaSaves[i].unused2 = in.read_int<u32>();
    }

    in.seekg(museum::GALAGA_COMPLETE_OFF + 4, in.beg);
    u32 tmp1;

    in.endian() = std::endian::little;
    tmp1 = in.read_int<u32>();
    in.endian() = std::endian::big;

    if(tmp1 == 1) g01complete = true;
    else g01complete = false;

    in.close();

    // std::copy(dataVec.begin(), dataVec.end(), data.str().begin());

    /*
    crc = crc32(0, 0, 0);

    for(int i = 0; i < dataVec.size(); i++) {
        crc = crc32(crc, reinterpret_cast<const unsigned char*>(&dataVec[0]) + i, 1);
    }

    in.open(droppedFilesVec[0]);
    if(!in.is_open()) {
        std::cerr << droppedFilesVec[0] << " not opened!\n";
        return 1;
    }

    in.write_int(crc);
    in.write(reinterpret_cast<char*>(&dataVec[0]), dataVec.size());

    in.close();
    */
}

int main(int argc, char **argv) {
    InitWindow(800, 600, "musave");

    FilePathList droppedFiles;
    bool droppedFile = false;

    std::vector<std::string> droppedFilesVec(1);

    estream in;
    in.endian() = std::endian::big;
    std::stringstream data;
    std::vector<char> dataVec(museum::SAVE_SIZE - 4);

    std::vector<std::string> g01worlds = {"1", "2", "3"};
    int g01worldIdx = 0;
    int g01stageIdx = 0;
    std::vector<std::string> sections = {"g01", "g02", "g03", "g04", "g07"};
    int sectionIdx = 0;
    std::vector<int> g01EnemiesCount = {139, 224, 278,
                                        286, 366, 193,
                                        320, 274, 118};
    bool g01complete;

    std::string input = "";
    std::string lastInput = "";
    bool doInput = false;
    bool doInitLoad = false;

    std::vector<struct museum::GalagaSave> galagaSaves(10);

    while(!WindowShouldClose()) {
        if(doInput) {
            int key = GetKeyPressed();
            char ch = GetCharPressed();

            if(key != 0) {
                input += ch;

                if(key == KEY_ENTER) {
                    lastInput = input;
                    doInput = false;
                    input = "";
                } else if(key == KEY_BACKSPACE) {
                    input.pop_back();
                } else if(key == KEY_END) {
                    lastInput = "";
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
                Load(in, droppedFilesVec, data, galagaSaves, g01complete);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if(doInput) {
            DrawText("INPUT: ", 20, 580, 20, WHITE);

            DrawText(TextFormat("%s", input.c_str()), 100, 580, 20, WHITE);
        }

        for(int i = 0; i < sections.size(); i++) {
            if(i == sectionIdx) {
                if(GuiButton({(float)(20 + (i * 100)), 20, 100, 70}, sections[i].c_str())) {
                    sectionIdx = i;
                }
            } else {
                if(GuiButton({(float)(20 + (i * 100)), 20, 100, 40}, sections[i].c_str())) {
                    sectionIdx = i;
                }
            }
        }

        if(doInitLoad) {
            if(GuiButton({560, 20, 100, 20}, "READ")) {
                Load(in, droppedFilesVec, data, galagaSaves, g01complete);
            }
            if(GuiButton({680, 20, 100, 20}, "WRITE")) {
                in.open(droppedFilesVec[0]);
                in.seekp(museum::GALAGA_OFF, in.beg);

                for(int i = 0; i < 10; i++) {
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
                    GuiGroupBox({20 + (float)(((int)(760 / 3)) * i), 140, ((int)(760 / 3)), (580 - 140) - 40}, TextFormat("%d - %d", g01worldIdx + 1, i + 1));
                }

                int g01stageVecIdx = g01StageIdx(g01worldIdx, g01stageIdx);
                for(int i = 0; i < 3; i++) {
                    int g01stageVecIdx = g01StageIdx(g01worldIdx, i);

                    if(GuiLabelButton({40 + (float)((int)(760 /3)) * i, 160, 100, 20}, TextFormat("High Score: %d", galagaSaves[g01stageVecIdx].score))) {
                        doInput = true;

                        if(lastInput != "") {
                            galagaSaves[g01stageVecIdx].score = std::stoi(lastInput);
                        }
                    }
                    if(galagaSaves[g01stageVecIdx].scoreBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i + 170, 160, 50, 20}, "PERFECT", GOLD)) {
                            galagaSaves[g01stageVecIdx].scoreBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i + 170, 160, 50, 20}, "PERFECT", GRAY)) {
                            galagaSaves[g01stageVecIdx].scoreBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(760 /3)) * i, 180, 100, 20}, TextFormat("Enemies Shot Down: %d / %d", galagaSaves[g01stageVecIdx].shot, g01EnemiesCount[g01stageVecIdx]))) {
                        doInput = true;

                        if(lastInput != "") {
                            galagaSaves[g01stageVecIdx].shot = std::stoi(lastInput);
                        }

                    }
                    if(galagaSaves[g01stageVecIdx].shotBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i + 170, 180, 50, 20}, "PERFECT", GOLD)) {
                            galagaSaves[g01stageVecIdx].shotBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i + 170, 180, 50, 20}, "PERFECT", GRAY)) {
                            galagaSaves[g01stageVecIdx].shotBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(760 /3)) * i, 200, 100, 20}, TextFormat("Accuracy: %.1f%%", (float)(galagaSaves[g01stageVecIdx].accuracy) / 10))) {
                        doInput = true;

                        if(lastInput != "") {
                            galagaSaves[g01stageVecIdx].accuracy = std::stof(lastInput) * 10;
                        }

                    }
                    if(galagaSaves[g01stageVecIdx].accuracyBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i + 170, 200, 50, 20}, "PERFECT", GOLD)) {
                            galagaSaves[g01stageVecIdx].accuracyBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i + 170, 200, 50, 20}, "PERFECT", GRAY)) {
                            galagaSaves[g01stageVecIdx].accuracyBadge = 1;
                        }
                    }

                    if(GuiLabelButton({40 + (float)((int)(760 /3)) * i, 220, 100, 20}, TextFormat("Defense: %d / 7", galagaSaves[g01stageVecIdx].hp))) {
                        doInput = true;

                        if(lastInput != "") {
                            galagaSaves[g01stageVecIdx].hp = std::stoi(lastInput);
                        }

                    }
                    if(galagaSaves[g01stageVecIdx].hpBadge) {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i + 170, 220, 50, 20}, "PERFECT", GOLD)) {
                            galagaSaves[g01stageVecIdx].hpBadge = 0;
                        }
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i + 170, 220, 50, 20}, "PERFECT", GRAY)) {
                            galagaSaves[g01stageVecIdx].hpBadge = 1;
                        }
                    }

                    if(galagaSaves[g01stageVecIdx].flag) {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i, 540, 200, 30}, "UNLOCKED", GOLD)) galagaSaves[g01stageVecIdx].flag = 0;
                    } else {
                        if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i, 540, 200, 30}, "LOCKED", GRAY)) galagaSaves[g01stageVecIdx].flag = 1;
                    }

                    if(i == 2 && g01worldIdx == 2) {
                        if(g01complete) {
                            if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i, 560, 100, 30}, "BOSS COMPLETE", GOLD)) {
                                g01complete = false;
                            }
                        } else {
                            if(GuiLabelButtonColor({40 + (float)((int)(760 /3)) * i, 560, 100, 30}, "BOSS COMPLETE", GRAY)) {
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
