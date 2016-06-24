#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define BINARY_SEGMENT1 2
#define BINARY_SEGMENT2 3
#define BINARY_SEGMENT3 14
#define BINARY_SEGMENT4 15

#define BLUE_BUTTON 5
#define GREEN_BUTTON 6
#define RED_BUTTON 7
#define YELLOW_BUTTON 8

#define BLUE_LED 9
#define GREEN_LED 10
#define RED_LED 11
#define YELLOW_LED 12

#define DEFAULT_BUZZER 13

#define LDR 16
#define LM35 17

const int MAX_SEQUENCE = 9;
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

void setDisplayMessage(String line1, String line2) {
  float weatherNow = analogRead(LM35) * 0.1075268817;

  line1 = (line1=="")?"WELCOME GAMER":line1;
  line2 = (line2=="")?"Weather Now: " + String((int) weatherNow) + "c":line2;

  Serial.println(line1);
  Serial.println(line2);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void setDisplaySegment(int number) {
  Serial.println(number);

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

void setColorMessage(int ledPort) {
  switch (ledPort) {
    case RED_LED:
      setDisplayMessage("RED (VERMELHO)","");

      break;
    case GREEN_LED:
      setDisplayMessage("GREEN (VERDE)","");

      break;
    case BLUE_LED:
      setDisplayMessage("BLUE (AZUL)","");

      break;
    case YELLOW_LED:
      setDisplayMessage("YELLOW (AMARELO)","");

      break;
  }
}

void setPlayTone(int frequence, int duration) {
  tone(DEFAULT_BUZZER, frequence, duration);
}

void setFlashLight(int ledPort, int waitingTime) {
  digitalWrite(ledPort, HIGH);
  delay(waitingTime);

  setPlayTone(ledPort * 400, 200);

  digitalWrite(ledPort, LOW);
  delay(waitingTime);
}

int getButtonPressed() {
  char keyPress = Serial.read();

  if (digitalRead(RED_BUTTON) == LOW || keyPress == 'R') {
    setColorMessage(RED_LED);
    setFlashLight(RED_LED, HOLD_ON);
    setDisplayMessage("NEXT COLOR","");

    return RED_LED;
  }

  if (digitalRead(GREEN_BUTTON) == LOW || keyPress == 'G') {
    setColorMessage(GREEN_LED);
    setFlashLight(GREEN_LED, HOLD_ON);
    setDisplayMessage("NEXT COLOR","");

    return GREEN_LED;
  }

  if (digitalRead(BLUE_BUTTON) == LOW || keyPress == 'B') {
    setColorMessage(BLUE_LED);
    setFlashLight(BLUE_LED, HOLD_ON);
    setDisplayMessage("NEXT COLOR","");

    return BLUE_LED;
  }

  if (digitalRead(YELLOW_BUTTON) == LOW || keyPress == 'Y') {
    setColorMessage(YELLOW_LED);
    setFlashLight(YELLOW_LED, HOLD_ON);
    setDisplayMessage("NEXT COLOR","");

    return YELLOW_LED;
  }

  return UNDEFINIED;
}

void playRoundSequence() {
  for (int i = 0; i < roundPlayer; i++) {
    setDisplaySegment(i+1);

    setColorMessage(gameSet[i]);

    setFlashLight(gameSet[i], HOLD_ON);

    setDisplayMessage("PLAY SEQUENCE","");
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
  setDisplayMessage("YOU WIN","Congratulations!");

  for (int i = 0; i < MAX_SEQUENCE + 1; i++) {
    setFlashLight(gameSet[i], 200);

    setDisplaySegment(MAX_SEQUENCE);
  }

  newGame();
}

void fail() {
  setDisplayMessage("YOU LOOSE!","Try Again...");

  setPlayTone(2048, 256);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);

  setDisplaySegment(0);

  delay(300);

  newGame();
}

void newGame() {
  roundPlayer = 0;
  rightAnswer = 0;

  randomSeed(analogRead(A7));

  for (int i = 0; i < MAX_SEQUENCE; i++) {
    gameSet[i] = random(BLUE_LED, YELLOW_LED + 1);
  }

  welcome();
}

void setup() {
  Serial.begin(9600);

  analogReference(INTERNAL);

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  pinMode(BINARY_SEGMENT1, OUTPUT);
  pinMode(BINARY_SEGMENT2, OUTPUT);
  pinMode(BINARY_SEGMENT3, OUTPUT);
  pinMode(BINARY_SEGMENT4, OUTPUT);

  pinMode(DEFAULT_BUZZER, OUTPUT);

  lcd.begin (16, 2);

  setDisplayMessage("WELCOME GAMER","decade.com.br");

  newGame();
}

void loop() {
  if (map(analogRead(LDR), 0, 1023, 0, 100) < 50) {
    lcd.setBacklight(HIGH);
  } else {
    lcd.setBacklight(LOW);
  }

  switch (getStatusGame()) {
    case READY_TO_PLAY:
      setRoundPlayer();
      break;
    case WAITING_RESPONSE:
      setRightAnswer();
      break;
    case YOU_WIN:
      congratulation();
      break;
    case GAME_OVER:
      fail();
      break;
  }
}