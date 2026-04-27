#include <Arduino.h>
#include "coordinates.h"

const float grid[5][5][2] = {
  { {-46,44}, {-23,44}, {0,44}, {23,44}, {46,44} },
  { {-46,22}, {-23,22}, {0,22}, {23,22}, {46,22} },
  { {-46,0},  {-23,0},  {0,0},  {23,0},  {46,0}  },
  { {-46,-22},{-23,-22},{0,-22},{23,-22},{46,-22} },
  { {-46,-44},{-23,-44},{0,-44},{23,-44},{46,-44} }
};

const float left_box[5][2] = {
  // {-80, 55},
  {-80, 33},
  {-80, 11},
  {-80, -11}, 
  {-80, -33},
  {-80, -55}
};

const float right_box[5][2] = {
  // {80, 55},
  {80, 33},
  {80, 11},
  {80, -11}, 
  {80, -33},
  {80, -55}
};

bool getCoordinate(char col, int row, float &x, float &y) {
  int colIndex = col - 'A';

  if (colIndex < 0 || colIndex > 4) {
    Serial.println("Invalid column. Please use A-E.");
    return false;
  }

  if (row < 1 || row > 5) {
    Serial.println("Invalid row number. Please use 1-5.");
    return false;
  }

  int rowIndex = row - 1;

  x = grid[rowIndex][colIndex][0];
  y = grid[rowIndex][colIndex][1];

  return true;
}

bool getLeftBox(int index, float &x, float &y) {
  if (index < 1 || index > 5) {
    Serial.println("Invalid left box index (1â5)");
    return false;
  }
  x = left_box[index - 1][0];
  y = left_box[index - 1][1];
  return true;
}

bool getRightBox(int index, float &x, float &y) {
  if (index < 1 || index > 5) {
    Serial.println("Invalid right box index (1â5)");
    return false;
  }
  x = right_box[index - 1][0];
  y = right_box[index - 1][1];
  return true;
}