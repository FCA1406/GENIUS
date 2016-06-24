#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define RED_BUTTON 2
#define GREEN_BUTTON 3
#define BLUE_BUTTON 4
#define YELLOW_BUTTON 5

#define RED_LED 8
#define GREEN_LED 9
#define BLUE_LED 10
#define YELLOW_LED 11

#define DEFAULT_BUZZER 13

#define BINARY_SEGMENT1 14
#define BINARY_SEGMENT2 15
#define BINARY_SEGMENT3 16
#define BINARY_SEGMENT4 17

const int MAX_SEQUENCE = 4;
const int HOLD_ON = 500;
const int UNDEFINIED = -1;

int gameSet[MAX_SEQUENCE];
int roundPlayer;
int rightAnswer;

enum statusGame {
  READY_TO_PLAY
, WAITING_RESPONSE
, YOU_WIN
, GAME_OVER
};

void setDisplayMessage(String text) {
  lcd.clear();

  //lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print(text);
  lcd.setCursor(0, 1);
  lcd.print("decade.com.br");
  lcd.setBacklight(LOW);
}

void setDisplayColor(int ledPort) {
  switch (ledPort) {
    case RED_LED:
      setDisplayMessage("RED (VERMELHO)");

      break;
    case GREEN_LED:
      setDisplayMessage("GREEN (VERDE)");

      break;
    case BLUE_LED:
      setDisplayMessage("WHITE (BRANCO)");

      break;
    case YELLOW_LED:
      setDisplayMessage("YELLOW (AMARELO)");

      break;
  }
}

void setDisplayNumber(int number) {
  switch (number) {
    case 0:
      digitalWrite(BINARY_SEGMENT1, LOW);
      digitalWrite(BINARY_SEGMENT2, LOW);
      digitalWrite(BINARY_SEGMENT3, LOW);
      digitalWrite(BINARY_SEGMENT4, LOW);

      break;
    case 1:
      digitalWrite(BINARY_SEGMENT1, HIGH);
      digitalWrite(BINARY_SEGMENT2, LOW);
      digitalWrite(BINARY_SEGMENT3, LOW);
      digitalWrite(BINARY_SEGMENT4, LOW);

      break;
    case 2:
      digitalWrite(BINARY_SEGMENT1, LOW);
      digitalWrite(BINARY_SEGMENT2, HIGH);
      digitalWrite(BINARY_SEGMENT3, LOW);
      digitalWrite(BINARY_SEGMENT4, LOW);

      break;
    case 3:
      digitalWrite(BINARY_SEGMENT1, HIGH);
      digitalWrite(BINARY_SEGMENT2, HIGH);
      digitalWrite(BINARY_SEGMENT3, LOW);
      digitalWrite(BINARY_SEGMENT4, LOW);

      break;
    case 4:
      digitalWrite(BINARY_SEGMENT1, LOW);
      digitalWrite(BINARY_SEGMENT2, LOW);
      digitalWrite(BINARY_SEGMENT3, HIGH);
      digitalWrite(BINARY_SEGMENT4, LOW);

      break;
    case 5:
      digitalWrite(BINARY_SEGMENT1, HIGH);
      digitalWrite(BINARY_SEGMENT2, LOW);
      digitalWrite(BINARY_SEGMENT3, HIGH);
      digitalWrite(BINARY_SEGMENT4, LOW);

      break;
    case 6:
      digitalWrite(BINARY_SEGMENT1, LOW);
      digitalWrite(BINARY_SEGMENT2, HIGH);
      digitalWrite(BINARY_SEGMENT3, HIGH);
      digitalWrite(BINARY_SEGMENT4, LOW);

      break;
    case 7:
      digitalWrite(BINARY_SEGMENT1, HIGH);
      digitalWrite(BINARY_SEGMENT2, HIGH);
      digitalWrite(BINARY_SEGMENT3, HIGH);
      digitalWrite(BINARY_SEGMENT4, LOW);

      break;
    case 8:
      digitalWrite(BINARY_SEGMENT1, LOW);
      digitalWrite(BINARY_SEGMENT2, LOW);
      digitalWrite(BINARY_SEGMENT3, LOW);
      digitalWrite(BINARY_SEGMENT4, HIGH);

      break;
    case 9:
      digitalWrite(BINARY_SEGMENT1, HIGH);
      digitalWrite(BINARY_SEGMENT2, LOW);
      digitalWrite(BINARY_SEGMENT3, LOW);
      digitalWrite(BINARY_SEGMENT4, HIGH);

      break;
  }
}

void setPlayTone(int frequence, int duration) {
  tone(DEFAULT_BUZZER, frequence, duration);
}

void setFlashLight(int ledPort, int waitingTime) {
  digitalWrite(ledPort, HIGH);
  delay(waitingTime);

  setPlayTone(ledPort * 512, 128);

  digitalWrite(ledPort, LOW);
  delay(waitingTime);
}

int getButtonPressed() {
  if (digitalRead(RED_BUTTON) == LOW) {
    setDisplayColor(RED_LED);
    setFlashLight(RED_LED, HOLD_ON);
    setDisplayMessage("NEXT COLOR");

    return RED_LED;
  }

  if (digitalRead(GREEN_BUTTON) == LOW) {
    setDisplayColor(GREEN_LED);
    setFlashLight(GREEN_LED, HOLD_ON);
    setDisplayMessage("NEXT COLOR");

    return GREEN_LED;
  }

  if (digitalRead(BLUE_BUTTON) == LOW) {
    setDisplayColor(BLUE_LED);
    setFlashLight(BLUE_LED, HOLD_ON);
    setDisplayMessage("NEXT COLOR");

    return BLUE_LED;
  }

  if (digitalRead(YELLOW_BUTTON) == LOW) {
    setDisplayColor(YELLOW_LED);
    setFlashLight(YELLOW_LED, HOLD_ON);
    setDisplayMessage("NEXT COLOR");

    return YELLOW_LED;
  }

  return UNDEFINIED;
}

void playRoundSequence() {
  setDisplayNumber(roundPlayer);

  for (int i = 0; i < roundPlayer; i++) {
    setDisplayColor(gameSet[i]);

    setFlashLight(gameSet[i], HOLD_ON);

    setDisplayMessage("PLAY SEQUENCE");
  }
}

void setRoundPlayer() {
  roundPlayer++;
  rightAnswer = 0;

  if (roundPlayer <= MAX_SEQUENCE) {
    playRoundSequence();
  }
}

void setRightAnswer() {
  int answer = getButtonPressed();

  if (answer == UNDEFINIED) {
    return;
  }

  if (answer == gameSet[rightAnswer]) {
    rightAnswer++;
  } else {
    roundPlayer = MAX_SEQUENCE + 2;
  }
}

int getStatusGame() {
  if (roundPlayer <= MAX_SEQUENCE) {
    if (rightAnswer == roundPlayer) {
      return READY_TO_PLAY;
    } else {
      return WAITING_RESPONSE;
    }
  } else if (roundPlayer == MAX_SEQUENCE + 1) {
    return YOU_WIN;
  } else {
    return GAME_OVER;
  }
}

void welcome() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);

  delay(400);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  delay(400);
}

void congratulation() {
  setDisplayMessage("YOU WIN!");

  for (int i = 0; i < MAX_SEQUENCE + 1; i++) {
    setFlashLight(gameSet[i], 200);

    setDisplayNumber(MAX_SEQUENCE);
  }

  newGame();
}

void fail() {
  setDisplayMessage("YOU LOOSE!");

  setPlayTone(2048, 256);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);

  setDisplayNumber(0);

  delay(300);

  newGame();
}

void newGame() {
  roundPlayer = 0;
  rightAnswer = 0;

  randomSeed(analogRead(A7));

  for (int i = 0; i < MAX_SEQUENCE; i++) {
    gameSet[i] = random(RED_LED, YELLOW_LED + 1);
  }

  welcome();
}

void setup() {
  //Serial.begin(9600);

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  pinMode(DEFAULT_BUZZER, OUTPUT);

  pinMode(BINARY_SEGMENT1, OUTPUT);
  pinMode(BINARY_SEGMENT2, OUTPUT);
  pinMode(BINARY_SEGMENT3, OUTPUT);
  pinMode(BINARY_SEGMENT4, OUTPUT);

  lcd.begin (16, 2);

  setDisplayMessage("WELCOME GAMER");

  newGame();
}

void loop() {
  switch (getStatusGame()) {
    case READY_TO_PLAY:
      //Serial.println("READY_TO_PLAY");

      setRoundPlayer();
      break;
    case WAITING_RESPONSE:
      //Serial.println("WAITING_RESPONSE");

      setRightAnswer();
      break;
    case YOU_WIN:
      //Serial.println("YOU_WIN");

      congratulation();
      break;
    case GAME_OVER:
      //Serial.println("GAME_OVER");

      fail();
      break;
  }
}