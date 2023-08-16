#pragma once

#include <unpac/estream.h>
#include <vector>
#include <string>
#include <sstream>
#include <unpac/museum.h>

int g01StageIdx(int worldIdx, int sectionIdx);
int g02StageIdx(int worldIdx, int sectionIdx);
int g03StageIdx(int worldIdx, int sectionIdx);
int g07StageIdx(int worldIdx, int sectionIdx);

/* fucking hell */
void Load(estream& in, std::vector<std::string>& droppedFilesVec,
          std::stringstream& data, std::vector<struct museum::GalagaSave>& galagaSaves,
          bool& g01complete, std::vector<struct museum::RallyXSave>& rallyXSaves,
          std::vector<struct museum::PacSave>& pacSaves, bool& g03complete,
          std::vector<struct museum::MotosSave>& motosSaves, bool& g07complete,
          int& g07lives, int& g07power, int& g07jump, int& g07charge);
