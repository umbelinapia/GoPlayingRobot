#include <Arduino.h>
#include "serial_commands.h"
#include "coordinates.h"
#include "tasks.h"

extern bool moveToBoardXY(float xb, float yb, bool elbowUp);
extern void endEffectorUp();
extern void endEffectorHover();
extern void endEffectorDown();

extern const float grid[5][5][2];
extern const float left_box[5][2];
extern const float right_box[5][2];

bool handleCellCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  float xb, yb;

  // End-effector commands
  if (cmd == "UP") {
    Serial.println("End effector -> UP");
    endEffectorUp();
    Serial.println("Done.");
  }
  else if (cmd == "HOVER") {
    Serial.println("End effector -> HOVER");
    endEffectorHover();
    Serial.println("Done.");
  }
  else if (cmd == "DOWN") {
    Serial.println("End effector -> DOWN");
    endEffectorDown();
    Serial.println("Done.");
  }

  // Named arm positions
  else if (cmd == "HOME") {
    xb = 0;
    yb = 60;
    moveToBoardXY(xb, yb, true);
    Serial.println("Moving to HOME...");
  }
  else if (cmd == "STANDBY") {
    xb = 200;
    yb = -140;
    moveToBoardXY(xb, yb, true);
    Serial.println("Moving to STANDBY...");
  }

  else if (cmd == "RESETBOX") {
    Serial.println("Resetting boxes...");
    initBoxes();
  }

  else if (cmd.length() > 1 && cmd.length() < 4) {

    char col = cmd.charAt(0);
    int row = cmd.charAt(1) - '0';

    // ===== GRID (AâE) =====
    if (col >= 'A' && col <= 'E') {
    
      if (!getCoordinate(col, row, xb, yb)) {
        return false;
      }
    }

    // ===== LEFT BOX (F) =====
    else if (col == 'F') {
    
      if (!getLeftBox(row, xb, yb)) {
        return false;
      }
    }

    // ===== RIGHT BOX (G) =====
    else if (col == 'G') {
    
      if (!getRightBox(row, xb, yb)) {
        return false;
      }
    }

    else {
      Serial.println("Invalid column. Use AâE (grid), F (left), G (right)");
      return false;
    }

    if (cmd.length() == 2) {
      moveToBoardXY(xb, yb, true);
    }

    // ===== pick and place Routines (+ R/P) =====
    else if (cmd.length() == 3) {

      char action = cmd.charAt(2);

      if (action == 'R') {
        RemoveStone(xb, yb);
      }
      else if (action == 'P') {
        PlaceStone(xb, yb);
      }
      else {
        Serial.println("Invalid action. Use R for remove or P for place");
        return false;
      }
    }
  }

  else {
    return false;
  }

  return true;
}

void readSerialCellCommand() {
  static String input = "";

  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (input.length() > 0) {
        handleCellCommand(input);
        input = "";
      }
    } else {
      input += c;
    }
  }
}