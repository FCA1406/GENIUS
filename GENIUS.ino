#define RED_BUTTON 2
#define GREEN_BUTTON 3
#define BLUE_BUTTON 4
#define YELLOW_BUTTON 5

#define RED_LED 8
#define GREEN_LED 9
#define BLUE_LED 10
#define YELLOW_LED 11

#define DEFAULT_BUZZER 13

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
    setFlashLight(RED_LED, HOLD_ON);

    return RED_LED;
  }

  if (digitalRead(GREEN_BUTTON) == LOW) {
    setFlashLight(GREEN_LED, HOLD_ON);

    return GREEN_LED;
  }

  if (digitalRead(BLUE_BUTTON) == LOW) {
    setFlashLight(BLUE_LED, HOLD_ON);

    return BLUE_LED;
  }

  if (digitalRead(YELLOW_BUTTON) == LOW) {
    setFlashLight(YELLOW_LED, HOLD_ON);

    return YELLOW_LED;
  }

  return UNDEFINIED;
}

void playRoundSequence() {
  for (int i = 0; i < roundPlayer; i++) {
    setFlashLight(gameSet[i], HOLD_ON);
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
  for (int i = 0; i < MAX_SEQUENCE + 1; i++) {
    setFlashLight(gameSet[i], 200);
  }

  newGame();
}

void fail() {
  setPlayTone(2048, 256);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);

  delay(300);

  newGame();
}

void newGame() {
  roundPlayer = 0;
  rightAnswer = 0;

  randomSeed(analogRead(0));

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