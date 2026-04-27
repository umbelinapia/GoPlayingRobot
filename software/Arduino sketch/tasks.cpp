#include <Arduino.h>
#include "tasks.h"
#include "coordinates.h"
#include "serial_commands.h"

extern bool moveToBoardXY(float xb, float yb, bool elbowUp);
extern void endEffectorUp();
extern void endEffectorHover();
extern void endEffectorDown();

extern const float grid[5][5][2];
extern const float left_box[5][2];
extern const float right_box[5][2];

const int BOX_SLOTS = 5;

bool rightBoxOccupied[BOX_SLOTS];
bool leftBoxOccupied[BOX_SLOTS];

void initBoxes() {
  for (int i = 0; i < BOX_SLOTS; i++) {
    rightBoxOccupied[i] = false;
    leftBoxOccupied[i] = true;
  }
}

bool getNextRightStone(float &x, float &y, int &slotIndex) {
  for (int i = 0; i < BOX_SLOTS; i++) {
    if (rightBoxOccupied[i]) {
      x = right_box[i][0];
      y = right_box[i][1];
      slotIndex = i;
      return true;
    }
  }
  return false;
}

bool getNextLeftStone(float &x, float &y, int &slotIndex) {
  for (int i = 0; i < BOX_SLOTS; i++) {
    if (leftBoxOccupied[i]) {
      x = left_box[i][0];
      y = left_box[i][1];
      slotIndex = i;
      return true;
    }
  }
  return false;
}

bool getNextEmptyLeftBox(float &x, float &y, int &slotIndex) {
  for (int i = 0; i < BOX_SLOTS; i++) {
    if (!leftBoxOccupied[i]) {
      x = left_box[i][0];
      y = left_box[i][1];
      slotIndex = i;
      return true;
    }
  }
  return false;
}

bool getNextEmptyRightBox(float &x, float &y, int &slotIndex) {
  for (int i = 0; i < BOX_SLOTS; i++) {
    if (!rightBoxOccupied[i]) {
      x = right_box[i][0];
      y = right_box[i][1];
      slotIndex = i;
      return true;
    }
  }
  return false;
}

void PlaceStone(float xb, float yb) {
  float sx, sy;
  int slotIndex;

  if (!getNextRightStone(sx, sy, slotIndex)) {
    Serial.println("No stones left in right box, checking left box...");
    if (!getNextLeftStone(sx, sy, slotIndex)) {
      Serial.println("No stones left in either box.");
      return;
    }

    Serial.print("Picking from left box F");
    Serial.println(slotIndex + 1);
    leftBoxOccupied[slotIndex] = false;
  } else {
    Serial.print("Picking from right box G");
    Serial.println(slotIndex + 1);
    rightBoxOccupied[slotIndex] = false;
  }

  moveToBoardXY(sx, sy, true);
  delay(2000);

  endEffectorDown();
  delay(2000);

  endEffectorHover();
  delay(2000);

  moveToBoardXY(xb, yb, true);
  delay(2000);

  endEffectorUp();
  delay(2000);

  Serial.println("Stone placed.");
    
  Serial.println("Returning to STANDBY...");
  handleCellCommand("STANDBY");  
  delay(2000);
}

void RemoveStone(float xb, float yb) {
  float dropX, dropY;
  int slotIndex;

  moveToBoardXY(xb, yb, true);
  delay(2000);

  endEffectorDown();
  delay(2000);

  endEffectorHover();
  delay(2000);

  if (getNextEmptyLeftBox(dropX, dropY, slotIndex)) {
    Serial.print("Dropping in left box F");
    Serial.println(slotIndex + 1);

    moveToBoardXY(dropX, dropY, true);
    delay(2000);

    leftBoxOccupied[slotIndex] = true;
  }
  else if (getNextEmptyRightBox(dropX, dropY, slotIndex)) {
    Serial.print("Dropping in right box G");
    Serial.println(slotIndex + 1);

    moveToBoardXY(dropX, dropY, true);
    delay(2000);

    rightBoxOccupied[slotIndex] = true;
  }
  else {
    Serial.println("No empty box slots available.");
    return;
  }
  
  endEffectorUp();

  Serial.println("Stone removed.");
  
  // Return to standby
  Serial.println("Returning to STANDBY...");
  handleCellCommand("STANDBY");  
  delay(2000);
}