//
// Created by SNAPDRAGON_11800H on 2021/11/24.
//

#include "Quaternion.h"

int Quaternion::index;
vector<vector<Intermediate *>>Quaternion::IC;
vector<string> Quaternion::str_G;

unordered_map<string, vector<sym>> Quaternion::tag_repr{
        {pair<string, vector<sym>>("getint", vector<sym>())},
        {pair<string, vector<sym>>("printf", vector<sym>())}
};