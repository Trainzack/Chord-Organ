#include <SD.h>
#include "Sequence.h"

//#define DEBUG_CHORDS

Sequence::Sequence(const char* filename) {
  _filename = filename;
}

void Sequence::init(boolean hasSD) {

  if (!hasSD) {
    // Configure defaults
    copyDefaults();
  } else {
    if (SD.exists(_filename)) {
      read();
    }
    else {

#ifdef DEBUG_MODE
      Serial.println("Settings file not found, writing new settings");
#endif
      write();
      read();
    };
  }
}

void Sequence::copyDefaults() {
  //TODO: Add better defaults
  sequence_len[0] = 12;
  int16_t seq[32][3] = {
    {0, 3, 16},
    {5, 3, 8},
    {0, 3, 8},
    {7, 1, 4},
    {5, 3, 4},
    {0, 3, 8},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
  };
  for (int i = 0; i < 32; i++) {
    for (int j = 0; j < 3; j++) {
      sequences[0][i][j] = seq[i][j];
    }
  }

}

void Sequence::read() {
  numSequences = 0;
  numChords = 0;

  char character;
  int value = 0;
  String settingValue;

  int NONE = 0;
  int CHORD = 1;
  int SETTING = 2;
  int state = NONE;

  int chord_index = 0;

  int time_steps = 0;

  sequenceFile = SD.open(_filename);

  while (sequenceFile.available()) {

    character = sequenceFile.read();


    if (character == '[') {
      if (numChords < 32 && numSequences > 0) {
        // Serial.println("Enter Chord");
        state = CHORD;
        continue;
      }
    } else if (character == '!') {
      state = SETTING;
    }

    if (state == CHORD) {
      if (character == ',') {
        sequences[numSequences - 1][numChords][chord_index] = settingValue.toInt();
        // If the user messes up and inputs too many values, the last one will be taken as duration
        chord_index = min(chord_index + 1, 2);
        settingValue = "";

      } else if (character == ']') {
        sequences[numSequences - 1][numChords][chord_index] = settingValue.toInt();
        settingValue = "";

        sequences[numSequences - 1][numChords][1] -= 1;
        numChords++;
        chord_index = 0;
        sequence_len[numSequences - 1] += sequences[numSequences - 1][numChords][2];
        // Serial.println("End Chord");
        state = NONE;
      } else {
        settingValue += character;
      }

    } else if (state == SETTING) {
      if (character == '\n') {
#ifdef DEBUG_CONFIG
        Serial.print("Config ");
        Serial.print(settingValue);
        Serial.println(".");
#endif
        int spacePos = settingValue.indexOf(' ');

        if (settingValue.startsWith("!S")) {
          if (numChords > 0 || numSequences <= 0) {
            //Don't create empty sequences!
            numSequences += 1;
            numChords = 0;
          }
        } else {
#ifdef DEBUG_CONFIG
          Serial.print("Unknown option:");
          Serial.print(settingValue);
          Serial.println(":");
#endif
        }
        settingValue = "";
        state = NONE;
      } else {
        settingValue += character;
      }
    }
  }
  sequenceFile.close();

  sequenceFile = SD.open("DEBUG.TXT", FILE_WRITE);
  //  // writing in the file works just like regular print()/println() function

  sequenceFile.println("DEBUG");

  sequenceFile.print("Sequences: ");
  sequenceFile.println(numSequences);
  sequenceFile.println("");
  for (int i = 0; i < numSequences; i++) {

    sequenceFile.print("Sequence #");
    sequenceFile.println((i + 1));
    for (int j = 0; j < 32; j++) {
      if (sequences[i][j][2] > 0) {
        sequenceFile.print("[");
        sequenceFile.print(sequences[i][j][0]);
        sequenceFile.print(", ");
        sequenceFile.print(sequences[i][j][1]);
        sequenceFile.print(", ");
        sequenceFile.print(sequences[i][j][2]);
        sequenceFile.println("]");
      }
    }
    sequenceFile.println("");
  }
  sequenceFile.print("Last Sequence Num Chords: ");
  sequenceFile.println(numChords);

  //
  // close the file:
  sequenceFile.close();
}


void Sequence::write() {
  // Delete the old One
  SD.remove(_filename);
  // Create new one
  sequenceFile = SD.open(_filename, FILE_WRITE);
  //  // writing in the file works just like regular print()/println() function

  sequenceFile.println("==========================");
  sequenceFile.println("|************************|");
  sequenceFile.println("|**====================**|");
  sequenceFile.println("|**|                  |**|");
  sequenceFile.println("|**|  CHORD SEQUENCE  |**|");
  sequenceFile.println("|**|                  |**|");
  sequenceFile.println("|**====================**|");
  sequenceFile.println("|************************|");
  sequenceFile.println("==========================");
  sequenceFile.println("");
  sequenceFile.println("This file is used when !SEQ is set in CHORDORG.txt");
  sequenceFile.println("");
  sequenceFile.println("Edit chord sequences in the space below.");
  sequenceFile.println("No more than 32 chords per sequence.");
  sequenceFile.println("No more than 16 sequences.");
  sequenceFile.println("Deliniate sequences with the !S marker.");
  sequenceFile.println("Anything outside the square brackets is ignored.");
  sequenceFile.println("The first number in the tuple is the root note");
  sequenceFile.println("of the chord.");
  sequenceFile.println("The second number in the tuple is which chord to use,");
  sequenceFile.println("as defined by CHORDORG.txt");
  sequenceFile.println("The third number in the tuple is how many beats to");
  sequenceFile.println("hold out the chord.");

  sequenceFile.println("");

  sequenceFile.println("!S 1: Blues");
  sequenceFile.println("1  [0,3,16] I^7");
  sequenceFile.println("2  [5,3,8] IV^7");
  sequenceFile.println("3  [0,3,8] I^7");
  sequenceFile.println("4  [7,3,4] V^7");
  sequenceFile.println("5  [5,3,4] IV^7");
  sequenceFile.println("7  [0,3,8] I^7");

  sequenceFile.println("!S 2: Minor Blues");
  sequenceFile.println("1  [0,4,16] Im7");
  sequenceFile.println("2  [5,4,8] IVm7");
  sequenceFile.println("3  [0,4,8] Im7");
  sequenceFile.println("4  [7,4,4] Vm7");
  sequenceFile.println("5  [5,4,4] IVm7");
  sequenceFile.println("7  [0,4,4] Im7");
  sequenceFile.println("7  [7,1,4] V");

  //
  // close the file:
  sequenceFile.close();
  //Serial.println("Writing done.");
}
