#pragma once
#include <Arduino.h>

bool getCoordinate(char col, int row, float &x, float &y);

bool getLeftBox(int index, float &x, float &y);
bool getRightBox(int index, float &x, float &y);

extern const float left_box[5][2];
extern const float right_box[5][2];
extern const float grid[5][5][2];


