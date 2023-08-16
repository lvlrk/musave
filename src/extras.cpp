#include "extras.h"

/* these functions are gross... */
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

int g02StageIdx(int worldIdx, int sectionIdx) {
    int stageIdx = 0;

    if(worldIdx == 0) {
        if(sectionIdx == 0) {
            stageIdx = 0;
        } else if(sectionIdx == 1) {
            stageIdx = 1;
        } else if(sectionIdx == 2) {
            stageIdx = 2;
        } else if(sectionIdx == 3) {
            stageIdx = 3;
        } else if(sectionIdx == 4) {
            stageIdx = 4;
        } else if(sectionIdx == 5) {
            stageIdx = 5; // redmode
        }
    } else if(worldIdx == 1) {
        if(sectionIdx == 0) {
            stageIdx = 6;
        } else if(sectionIdx == 1) {
            stageIdx = 7;
        } else if(sectionIdx == 2) {
            stageIdx = 8;
        } else if(sectionIdx == 3) {
            stageIdx = 9;
        } else if(sectionIdx == 4) {
            stageIdx = 10;
        } else if(sectionIdx == 5) {
            stageIdx = 11; // redmode
        }
    } else if(worldIdx == 2) {
        if(sectionIdx == 0) {
            stageIdx = 12;
        } else if(sectionIdx == 1) {
            stageIdx = 13;
        } else if(sectionIdx == 2) {
            stageIdx = 14;
        } else if(sectionIdx == 3) {
            stageIdx = 15;
        } else if(sectionIdx == 4) {
            stageIdx = 16;
        } else if(sectionIdx == 5) {
            stageIdx = 17; // redmode
        }
    } else if(worldIdx == 3) {
        if(sectionIdx == 0) {
            stageIdx = 18;
        } else if(sectionIdx == 1) {
            stageIdx = 19;
        } else if(sectionIdx == 2) {
            stageIdx = 20;
        } else if(sectionIdx == 3) {
            stageIdx = 21;
        } else if(sectionIdx == 4) {
            stageIdx = 22;
        } else if(sectionIdx == 5) {
            stageIdx = 23; // redmode
        }
    }

    return stageIdx;
}

int g03StageIdx(int worldIdx, int sectionIdx) {
    int stageIdx = 0;

    if(worldIdx == 0) {
        if(sectionIdx == 0) stageIdx = 0;
        else if(sectionIdx == 1) stageIdx = 1;
        else if(sectionIdx == 2) stageIdx = 2;
        else if(sectionIdx == 3) stageIdx = 3;
        else if(sectionIdx == 4) stageIdx = 4;
    } else if(worldIdx == 1) {
        if(sectionIdx == 0) stageIdx = 5;
        else if(sectionIdx == 1) stageIdx = 6;
        else if(sectionIdx == 2) stageIdx = 7;
        else if(sectionIdx == 3) stageIdx = 8;
    } else if(worldIdx == 2) {
        if(sectionIdx == 0) stageIdx = 9;
        else if(sectionIdx == 1) stageIdx = 10;
        else if(sectionIdx == 2) stageIdx = 11;
        else if(sectionIdx == 3) stageIdx = 12;
        else if(sectionIdx == 4) stageIdx = 13;
    } else if(worldIdx == 3) {
        if(sectionIdx == 0) stageIdx = 14;
        else if(sectionIdx == 1) stageIdx = 15;
        else if(sectionIdx == 2) stageIdx = 16;
        else if(sectionIdx == 3) stageIdx = 17;
        else if(sectionIdx == 4) stageIdx = 18;
        else if(sectionIdx == 5) stageIdx = 19;
    } else if(worldIdx == 4) {
        if(sectionIdx == 0) stageIdx = 20;
        else if(sectionIdx == 1) stageIdx = 21;
        else if(sectionIdx == 2) stageIdx = 22;
        else if(sectionIdx == 3) stageIdx = 23;
        else if(sectionIdx == 4) stageIdx = 24;
        else if(sectionIdx == 5) stageIdx = 25;
    }

    return stageIdx;
}

int g07StageIdx(int worldIdx, int sectionIdx) {
    int stageIdx = 0;

    if(worldIdx == 0) {
        if(sectionIdx == 0) stageIdx = 0;
        if(sectionIdx == 1) stageIdx = 1;
        if(sectionIdx == 2) stageIdx = 2;
        if(sectionIdx == 3) stageIdx = 3;
        if(sectionIdx == 4) stageIdx = 4;
        if(sectionIdx == 5) stageIdx = 5;
        if(sectionIdx == 6) stageIdx = 6;
    } else if(worldIdx == 1) {
        if(sectionIdx == 0) stageIdx = 7;
        if(sectionIdx == 1) stageIdx = 8;
        if(sectionIdx == 2) stageIdx = 9;
        if(sectionIdx == 3) stageIdx = 10;
        if(sectionIdx == 4) stageIdx = 11;
        if(sectionIdx == 5) stageIdx = 12;
        if(sectionIdx == 6) stageIdx = 13;
    } else if(worldIdx == 2) {
        if(sectionIdx == 0) stageIdx = 14;
        if(sectionIdx == 1) stageIdx = 15;
        if(sectionIdx == 2) stageIdx = 16;
        if(sectionIdx == 3) stageIdx = 17;
        if(sectionIdx == 4) stageIdx = 18;
        if(sectionIdx == 5) stageIdx = 19;
        if(sectionIdx == 6) stageIdx = 20;
    } else if(worldIdx == 3) {
        if(sectionIdx == 0) stageIdx = 21;
        if(sectionIdx == 1) stageIdx = 22;
        if(sectionIdx == 2) stageIdx = 23;
        if(sectionIdx == 3) stageIdx = 24;
        if(sectionIdx == 4) stageIdx = 25;
        if(sectionIdx == 5) stageIdx = 26;
        if(sectionIdx == 6) stageIdx = 27;
    } else if(worldIdx == 4) {
        if(sectionIdx == 0) stageIdx = 28;
        if(sectionIdx == 1) stageIdx = 29;
        if(sectionIdx == 2) stageIdx = 30;
        if(sectionIdx == 3) stageIdx = 31;
        if(sectionIdx == 4) stageIdx = 32;
        if(sectionIdx == 5) stageIdx = 33;
        if(sectionIdx == 6) stageIdx = 34;
    }

    return stageIdx;
}

/* jesus christ */
void Load(estream& in, std::vector<std::string>& droppedFilesVec,
          std::stringstream& data, std::vector<struct museum::GalagaSave>& galagaSaves,
          bool& g01complete, std::vector<struct museum::RallyXSave>& rallyXSaves,
          std::vector<struct museum::PacSave>& pacSaves, bool& g03complete,
          std::vector<struct museum::MotosSave>& motosSaves, bool& g07complete,
          int& g07lives, int& g07power, int& g07jump, int& g07charge) {
    in.open(droppedFilesVec[0]);
    if(!in.is_open()) {
        std::cerr << droppedFilesVec[0] << " not opened!\n";
        return;
    };

    u32 crc = in.read_int<u32>();

    data << in.rdbuf();

    in.seekg(museum::GALAGA_OFF, in.beg);
    for(int i = 0; i < 9; i++) {
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

    in.seekg(museum::RALLYX_OFF, in.beg);
    for(int i = 0; i < 25; i++) {
        rallyXSaves[i].time = in.read_int<u32>();
        rallyXSaves[i].score = in.read_int<u32>();
        rallyXSaves[i].scoreBadge = in.read_int<u32>();
        rallyXSaves[i].timeBadge = in.read_int<u32>();
        rallyXSaves[i].flag = in.read_int<u32>();
    }

    in.seekg(museum::PAC_OFF, in.beg);
    for(int i = 0; i < 26; i++) {
        pacSaves[i].time = in.read_int<u32>();
        pacSaves[i].unused1 = in.read_int<u32>();
        pacSaves[i].collected = in.read_int<u32>();
        pacSaves[i].max = in.read_int<u32>();
        pacSaves[i].unused2 = in.read_int<u32>();
        pacSaves[i].timeBadge = in.read_int<u32>();
        pacSaves[i].ghostBadge = in.read_int<u32>();
        pacSaves[i].collectedBadge = in.read_int<u32>();
        pacSaves[i].flag = in.read_int<u32>();
    }

    if(pacSaves[25].flag) g03complete = true;
    else g03complete = false;

    in.seekg(museum::MOTOS_OFF, in.beg);
    for(int i = 0; i < 35; i++) {
        motosSaves[i].score = in.read_int<u32>();
        motosSaves[i].time = in.read_int<u32>();
        motosSaves[i].scoreBadge = in.read_int<u32>();
        motosSaves[i].timeBadge = in.read_int<u32>();
        motosSaves[i].unused = in.read_int<u32>();
        motosSaves[i].flag = in.read_int<u32>();
    }

    in.seekg(museum::MOTOS_LIVES_OFF, in.beg);
    g07lives = in.read_int<u32>();
    in.seekg(museum::MOTOS_POWER_OFF, in.beg);
    g07power = in.read_int<u32>();
    g07jump = in.read_int<u32>();
    g07charge = in.read_int<u32>();

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
