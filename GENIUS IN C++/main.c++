#include <Arduino.h>

// Microcontroller Pin Mapping Constants
const int redLedPin = 13;                
const int blueLedPin = 12;   
const int yellowLedPin = 14;
const int greenLedPin = 15;             

const int redButtonPin = 33;              
const int blueButtonPin = 32;  
const int yellowButtonPin = 25;
const int greenButtonPin = 26;           

// Anti-bounce state flags for button debouncing
int redButtonFlag = 0;
int blueButtonFlag = 0;
int yellowButtonFlag = 0;
int greenButtonFlag = 0;

const int maxRounds = 15;         
int sequenceMemory[maxRounds];     
int playerMemory[maxRounds];

int gameStateMode = 0; // 0: Waiting for Start | 1: Gameplay Active
int currentRound = 1;                    
int difficultyDelayMs = 750;       

// Forward Declarations of functions
void executeIntroSequence();
void generateSequence();
void playGameTurn(int round);
void readPlayerTurn(int round);
bool verifyPlayerInputs(int round);
void turnOnAllLeds();
void turnOffAllLeds();
void flashAllLeds(int count);
void playLossAnimation();
void playVictoryAnimation();
bool checkRedButton();
bool checkBlueButton();
bool checkYellowButton();
bool checkGreenButton();

void setup() {
  Serial.begin(9600);
  
  // Seed the random number generator using floating analog environmental noise
  randomSeed(analogRead(0));
  
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  pinMode(redButtonPin, INPUT_PULLUP);
  pinMode(blueButtonPin, INPUT_PULLUP);
  pinMode(yellowButtonPin, INPUT_PULLUP);
  pinMode(greenButtonPin, INPUT_PULLUP);
}

void loop() {
  executeIntroSequence();
  
  flashAllLeds(3);
  delay(1000);
  
  // Sequential round processing loops
  for (currentRound = 1; currentRound <= maxRounds; currentRound++) {
    playGameTurn(currentRound);
    readPlayerTurn(currentRound);

    if (!verifyPlayerInputs(currentRound)) {
      Serial.println(F("Incorrect sequence! Game Over."));
      playLossAnimation();
      break; 
    } 
    else if (currentRound == maxRounds) {
      Serial.println(F("Flawless run! You win!"));
      playVictoryAnimation();
    } 
    else {
      turnOnAllLeds();
      delay(1000);
      turnOffAllLeds();
    }

    delay(200);
  }  

  gameStateMode = 0; // Revert state back to standby sequence
}

void executeIntroSequence() {
  turnOnAllLeds();
  
  while (gameStateMode == 0) {
    if (checkRedButton() || checkBlueButton() || checkYellowButton() || checkGreenButton()) {
      gameStateMode = 1;
    }
  }

  generateSequence();
}

void generateSequence() {
  for (int i = 0; i < maxRounds; i++) {
    sequenceMemory[i] = random(0, 4); // Generates indexes 0, 1, 2, or 3
  }
}

bool checkRedButton() {
  if (digitalRead(redButtonPin) == LOW && redButtonFlag == 0) { 
    redButtonFlag = 1;
    delay(7); // Debounce physical contacts settling delay
    return true;
  } 
  else if (digitalRead(redButtonPin) == HIGH && redButtonFlag == 1) { 
    redButtonFlag = 0;
    delay(7);
    return false;
  }
  return false;
}

bool checkBlueButton() {
  if (digitalRead(blueButtonPin) == LOW && blueButtonFlag == 0) { 
    blueButtonFlag = 1;
    delay(7);
    return true;
  } 
  else if (digitalRead(blueButtonPin) == HIGH && blueButtonFlag == 1) { 
    blueButtonFlag = 0;
    delay(7);
    return false;
  }
  return false;
}

bool checkYellowButton() {
  if (digitalRead(yellowButtonPin) == LOW && yellowButtonFlag == 0) { 
    yellowButtonFlag = 1;
    delay(7);
    return true;
  } 
  else if (digitalRead(yellowButtonPin) == HIGH && yellowButtonFlag == 1) { 
    yellowButtonFlag = 0;
    delay(7);
    return false;
  }
  return false;
}

bool checkGreenButton() {
  if (digitalRead(greenButtonPin) == LOW && greenButtonFlag == 0) { 
    greenButtonFlag = 1;
    delay(7);
    return true;
  } 
  else if (digitalRead(greenButtonPin) == HIGH && greenButtonFlag == 1) { 
    greenButtonFlag = 0;
    delay(7);
    return false;
  }
  return false;
}

void playGameTurn(int round) {
  Serial.print(F("CPU Turn: Round "));
  Serial.println(round);
  
  turnOffAllLeds();
  
  for (int i = 0; i < round; i++) {
    if (sequenceMemory[i] == 0)      digitalWrite(redLedPin, HIGH);
    else if (sequenceMemory[i] == 1) digitalWrite(blueLedPin, HIGH);
    else if (sequenceMemory[i] == 2) digitalWrite(yellowLedPin, HIGH);
    else if (sequenceMemory[i] == 3) digitalWrite(greenLedPin, HIGH);

    delay(difficultyDelayMs);
    turnOffAllLeds();
    delay(difficultyDelayMs / 2);
  }
}

void readPlayerTurn(int round) {
  Serial.print(F("Player Turn: Repeat "));
  Serial.println(round);
  
  int inputsRecorded = 0;
  
  while (inputsRecorded < round) {
    turnOffAllLeds();
    
    if (checkRedButton()) {
      Serial.println(F("Red Pressed"));
      playerMemory[inputsRecorded] = 0;
      digitalWrite(redLedPin, HIGH);
      delay(200);
      inputsRecorded++;
    }
    else if (checkBlueButton()) {
      Serial.println(F("Blue Pressed"));
      playerMemory[inputsRecorded] = 1;
      digitalWrite(blueLedPin, HIGH);
      delay(200);
      inputsRecorded++;
    }
    else if (checkYellowButton()) {
      Serial.println(F("Yellow Pressed"));
      playerMemory[inputsRecorded] = 2;
      digitalWrite(yellowLedPin, HIGH);
      delay(200);
      inputsRecorded++;
    }
    else if (checkGreenButton()) {
      Serial.println(F("Green Pressed"));
      playerMemory[inputsRecorded] = 3;
      digitalWrite(greenLedPin, HIGH);
      delay(200);
      inputsRecorded++;
    }
  }
  
  delay(1000);
  turnOffAllLeds();
}

bool verifyPlayerInputs(int round) {
  for (int i = 0; i < round; i++) {
    if (sequenceMemory[i] != playerMemory[i]) {
      return false;
    }
  }
  return true;
}

void turnOnAllLeds() {
  digitalWrite(redLedPin, HIGH);
  digitalWrite(blueLedPin, HIGH);
  digitalWrite(yellowLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
}

void turnOffAllLeds() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(blueLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
}

void flashAllLeds(int count) {
  for (int i = 0; i < count; i++) {
    turnOnAllLeds();
    delay(500);
    turnOffAllLeds();
    delay(500); 
  }
}

void playLossAnimation() {
  for (int i = 0; i < 15; i++) {
    turnOnAllLeds();
    delay(100);
    turnOffAllLeds();
    delay(100); 
  }
}

void playVictoryAnimation() {
  turnOffAllLeds();
  
  for (int i = 0; i < 16; i++) {
    int channelStep = i % 4;
    digitalWrite(redLedPin,    (channelStep == 0) ? HIGH : LOW);
    digitalWrite(blueLedPin,   (channelStep == 1) ? HIGH : LOW);
    digitalWrite(yellowLedPin, (channelStep == 2) ? HIGH : LOW);
    digitalWrite(greenLedPin,  (channelStep == 3) ? HIGH : LOW);
    delay(200);
  }
  turnOffAllLeds();
}