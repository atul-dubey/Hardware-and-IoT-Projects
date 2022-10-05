#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1);

int resolution[2] = {128, 64}, ball[2] = {20, (resolution[1] / 2)};
const int PIXEL_SIZE = 8, WALL_WIDTH = 4, PADDLE_WIDTH = 4, BALL_SIZE = 4, SPEED = 3;
int playerScore = 0, aiScore = 0, playerPos = 0, aiPos = 0;
char ballDirectionHori = 'R', ballDirectionVerti = 'S';
boolean inProgress = true;

void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
}

void loop() {
  if (aiScore > 9 || playerScore > 9) {
    // check game state
    inProgress = false;
  }

  if (inProgress) {
    eraseScore();
    eraseBall(ball[0], ball[1]);

    if (ballDirectionVerti == 'U') {
      // move ball up diagonally
      ball[1] = ball[1] - SPEED;
    }

    if (ballDirectionVerti == 'D') {
      // move ball down diagonally
      ball[1] = ball[1] + SPEED;
    }

    if (ball[1] <= 0) { // bounce the ball off the top ballDirectionVerti = 'D'; } if (ball[1] >= resolution[1]) {
      // bounce the ball off the bottom
      ballDirectionVerti = 'U';
    }

    if (ballDirectionHori == 'R') {
      ball[0] = ball[0] + SPEED; // move ball
      if (ball[0] >= (resolution[0] - 6)) {
        // ball is at the AI edge of the screen
        if ((aiPos + 12) >= ball[1] && (aiPos - 12) <= ball[1]) { // ball hits AI paddle if (ball[1] > (aiPos + 4)) {
            // deflect ball down
            ballDirectionVerti = 'D';
          }
          else if (ball[1] < (aiPos - 4)) {
            // deflect ball up
            ballDirectionVerti = 'U';
          }
          else {
            // deflect ball straight
            ballDirectionVerti = 'S';
          }
          // change ball direction
          ballDirectionHori = 'L';
        }
        else {
          // GOAL!
          ball[0] = 6; // move ball to other side of screen
          ballDirectionVerti = 'S'; // reset ball to straight travel
          ball[1] = resolution[1] / 2; // move ball to middle of screen
          ++playerScore; // increase player score
        }
      }
    }

    if (ballDirectionHori == 'L') {
      ball[0] = ball[0] - SPEED; // move ball
      if (ball[0] <= 6) { // ball is at the player edge of the screen if ((playerPos + 12) >= ball[1] && (playerPos - 12) <= ball[1]) { // ball hits player paddle if (ball[1] > (playerPos + 4)) {
            // deflect ball down
            ballDirectionVerti = 'D';
          }
          else if (ball[1] < (playerPos - 4)) { // deflect ball up ballDirectionVerti = 'U'; } else { // deflect ball straight ballDirectionVerti = 'S'; } // change ball direction ballDirectionHori = 'R'; } else { ball[0] = resolution[0] - 6; // move ball to other side of screen ballDirectionVerti = 'S'; // reset ball to straight travel ball[1] = resolution[1] / 2; // move ball to middle of screen ++aiScore; // increase AI score } } } drawBall(ball[0], ball[1]); erasePlayerPaddle(playerPos); playerPos = analogRead(A2); // read player potentiometer playerPos = map(playerPos, 0, 1023, 8, 54); // convert value from 0 - 1023 to 8 - 54 drawPlayerPaddle(playerPos); moveAi(); drawNet(); drawScore(); } else { // somebody has won display.clearDisplay(); display.setTextSize(4); display.setTextColor(WHITE); display.setCursor(0, 0); // figure out who if (aiScore > playerScore) {
      display.println("YOU  LOSE!");
    }
    else if (playerScore > aiScore) {
      display.println("YOU  WIN!");
    }
  }

  display.display();
}

void moveAi() {
  // move the AI paddle
  eraseAiPaddle(aiPos);
  if (ball[1] > aiPos) {
    ++aiPos;
  }
  else if (ball[1] < aiPos) {
    --aiPos;
  }
  drawAiPaddle(aiPos);
}

void drawScore() {
  // draw AI and player scores
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(45, 0);
  display.println(playerScore);

  display.setCursor(75, 0);
  display.println(aiScore);
}

void eraseScore() {
  // erase AI and player scores
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(45, 0);
  display.println(playerScore);

  display.setCursor(75, 0);
  display.println(aiScore);
}

void drawNet() {
  for (int i = 0; i < (resolution[1] / WALL_WIDTH); ++i) {
    drawPixel(((resolution[0] / 2) - 1), i * (WALL_WIDTH) + (WALL_WIDTH * i), WALL_WIDTH);
  }
}

void drawPixel(int posX, int posY, int dimensions) {
  // draw group of pixels
  for (int x = 0; x < dimensions; ++x) {
    for (int y = 0; y < dimensions; ++y) {
      display.drawPixel((posX + x), (posY + y), WHITE);
    }
  }
}

void erasePixel(int posX, int posY, int dimensions) {
  // erase group of pixels
  for (int x = 0; x < dimensions; ++x) {
    for (int y = 0; y < dimensions; ++y) {
      display.drawPixel((posX + x), (posY + y), BLACK);
    }
  }
}

void erasePlayerPaddle(int row) {
  erasePixel(0, row - (PADDLE_WIDTH * 2), PADDLE_WIDTH);
  erasePixel(0, row - PADDLE_WIDTH, PADDLE_WIDTH);
  erasePixel(0, row, PADDLE_WIDTH);
  erasePixel(0, row + PADDLE_WIDTH, PADDLE_WIDTH);
  erasePixel(0, row + (PADDLE_WIDTH + 2), PADDLE_WIDTH);
}

void drawPlayerPaddle(int row) {
  drawPixel(0, row - (PADDLE_WIDTH * 2), PADDLE_WIDTH);
  drawPixel(0, row - PADDLE_WIDTH, PADDLE_WIDTH);
  drawPixel(0, row, PADDLE_WIDTH);
  drawPixel(0, row + PADDLE_WIDTH, PADDLE_WIDTH);
  drawPixel(0, row + (PADDLE_WIDTH + 2), PADDLE_WIDTH);
}

void drawAiPaddle(int row) {
  int column = resolution[0] - PADDLE_WIDTH;
  drawPixel(column, row - (PADDLE_WIDTH * 2), PADDLE_WIDTH);
  drawPixel(column, row - PADDLE_WIDTH, PADDLE_WIDTH);
  drawPixel(column, row, PADDLE_WIDTH);
  drawPixel(column, row + PADDLE_WIDTH, PADDLE_WIDTH);
  drawPixel(column, row + (PADDLE_WIDTH * 2), PADDLE_WIDTH);
}

void eraseAiPaddle(int row) {
  int column = resolution[0] - PADDLE_WIDTH;
  erasePixel(column, row - (PADDLE_WIDTH * 2), PADDLE_WIDTH);
  erasePixel(column, row - PADDLE_WIDTH, PADDLE_WIDTH);
  erasePixel(column, row, PADDLE_WIDTH);
  erasePixel(column, row + PADDLE_WIDTH, PADDLE_WIDTH);
  erasePixel(column, row + (PADDLE_WIDTH * 2), PADDLE_WIDTH);
}

void drawBall(int x, int y) {
  display.drawCircle(x, y, BALL_SIZE, WHITE);
}

void eraseBall(int x, int y) {
  display.drawCircle(x, y, BALL_SIZE, BLACK);
}
