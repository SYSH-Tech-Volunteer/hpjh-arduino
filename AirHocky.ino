#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Ball Size
const byte VRX1=A0,VRY1=A2,VRX2=A2,VRY2=A3,R=4;
byte score1,score2,ballX,ballY,x1,y1,x2,y2,lx1,lx2,ly1,ly2,delX,delY;
void setup(){
  pinMode(VRX1,INPUT);
  pinMode(VRY1,INPUT);
  pinMode(VRX2,INPUT);
  pinMode(VRY2,INPUT);
}
void loop(){
  score1=score2=0;
  ballX=64;
  // Start Screen
  display.display();
  delay(1000);
  // Draw Arrow
  for (byte i = 0; i < 2; i++) {
    for (byte i = 0; i < 41; i++) {
      display.fillRect(32,105-i,16,i,1);
      delay(3);
    }
    for (byte i = 65; i > 46; i--) {
      display.fillTriangle(40,i,24,65,56,65,1);
      delay(3);
    }
  }

  // Wait for both players to be ready (Push Foward)
  int p1, p2;
  while (analogRead(VRY1) > 100 || analogRead(VRY2) < 923) {
    // Player 1
    if (analogRead(VRY1) - p1 > 5) {
      display.fillRect(0, 25, 80, 80, BLACK);
      display.fillRect(32, 65, 16, 40, ARROW);
      display.fillTriangle(40, 45, 24, 65, 56, 65, ARROW);
    }
    p1 = analogRead(VRY1);
    if (analogRead(VRY1) < 234)
      display.fillRect(32, 65, 16, 40, WHITE);
    else
      display.fillRect(32, analogRead(VRY1) * 40 / 166 + 8.614457831325296, 16, analogRead(VRY1) * -40 / 166 + 96.3855421686747, WHITE);
    if (234 - analogRead(VRY1) > 5)
      display.fillTriangle(40, analogRead(VRY1) * 20 / 134 + 30.07462686567164, 24, 65, 56, 65, WHITE);

    // Player 2
    if (p2 - analogRead(VRY2) > 5) {
      display.fillRect(0, 25, 80, 80, BLACK);
      display.fillRect(32, 65, 16, 40, ARROW);
      display.fillTriangle(40, 45, 24, 65, 56, 65, ARROW);
    }
    p2 = analogRead(VRY2);
    if (analogRead(VRY2) > 844)
      display.fillRect(32, 65, 16, 40, WHITE);
    else
      display.fillRect(32, analogRead(VRY2) * -40 / 332 + 166.6867469879518, 16, analogRead(VRY2) * 40 / 332 - 61.68674698795181, WHITE);
    if (analogRead(VRY2) > 844)
      display.fillTriangle(40, analogRead(VRY2) * -20 / 66 + 320.75757575757575, 24, 65, 56, 65, WHITE);
  }

  // Game Start
  while (score1 < 3 && score2 < 3) {  // Score Limit
    display.fillScreen(BLACK);
    delX = delY = 0;
    ballY = 65;  // Locate Ball
    display.fillCircle(x1, y1, r, PONE);
    displsy.fillCircle(x2, y2, r, PTWO);
    display.fillCircle(ballX, ballY, r, BALL);
    while (ballX >= r && ballX <= 160 - r || ballY <= 55 - r || ballY > 75 + r) {  // Ball in the area
      area();
      x1 = analogRead(VRX1) * 75.0 / 1023 + 5;  // Locate Joystick 1
      y1 = analogRead(VRY1) * 70.0 / 1023 + 30;
      if (byte(lx1) != byte(x1) || byte(ly1) != byte(y1)) {
        tft.fillCircle(lx1, ly1, r, BLACK);
        area();
        tft.fillCircle(x1, y1, r, PONE);
        lx1 = x1;
        ly1 = y1;
      }
      x2 = analogRead(VRX2) * 75.0 / 1023 + 80;  // Locate Joystick 2
      y2 = analogRead(VRY2) * 70.0 / 1023 + 30;
      if (byte(lx2) != byte(x2) || byte(ly2) != byte(y2)) {
        tft.fillCircle(lx2, ly2, r, BLACK);
        area();
        tft.fillCircle(x2, y2, r, PTWO);
        lx2 = x2;
        ly2 = y2;
      }
      if (abs(ballX - x1) <= 10 && abs(ballY - y1) <= 10) {  // Player 1 touch the ball
        delX = (ballX - x1) / 10;
        delY = (ballY - y1) / 10;
      }
      if (abs(ballX - x2) <= 10 && abs(ballY - y2) <= 10) {  // Player 2 touch the ball
        delX = (ballX - x2) / 10;
        delY = (ballY - y2) / 10;
      }
      if (delX || delY) {  // Ball move
        tft.fillCircle(ballX, ballY, r, BLACK);
        ballX += delX;
        ballY += delY;
        tft.fillCircle(ballX, ballY, r, BALL);
      }
      if (ballX < r)
        delX = abs(delX);  // Ball touch the left, delX always positive
      if (ballX > 160 - r)
        delX = -abs(delX);  // Ball touch the right, delX always negative
      if (ballY < 25 + r)
        delY = abs(delY);  // Ball touch the up, delY always positive
      if (ballY > 105 - r)
        delY = -abs(delY);  // Ball touch the down, delY always negative
    }
    byte side = 80;
    if (ballX < 80) {  // Player 1 score
      score1++;
      ballX = 60;
    } else {  // Player 2 score
      score2++;
      ballX = 100;
      side = 0;
    }

    // Score animation
    tft.setTextSize(4);
    tft.setTextColor(WHITE);
    for (int i = 0; i < 2; i++) {
      tft.setRotation(3);
      tft.fillRect(side, 25, 80, 80, DARK);
      tft.setCursor(30, 60);
      tft.println(score2);
      tft.setRotation(1);
      tft.setCursor(30, 60);
      tft.println(score1);
      delay(500);
      tft.setRotation(3);
      tft.fillRect(side, 25, 80, 80, BLACK);
      area();
      tft.setCursor(30, 60);
      tft.println(score2);
      tft.setRotation(1);
      tft.setCursor(30, 60);
      tft.println(score1);
      delay(500);
    }
  }

  // Game end animation
  tft.fillScreen(BLACK);
  byte winner = 2;
  if (score1 > 2)
    winner = 1;
  tft.setRotation(winner * 2 - 1);
  tft.setTextSize(2);
  tft.setCursor(10, 40);
  tft.setTextColor(WHITE);
  tft.print("player");
  tft.print(winner);
  tft.print(" Win");
  for (int i = 1; i < 26; i++) {
    tft.fillRect(0, 105 - i, 160, i, DARK);
  }
  delay(2000);
}

// Draw the area
void area() {
  tft.drawRect(0, 55, 2, 20, FIELD);    // Goal 1
  tft.drawRect(158, 55, 2, 20, FIELD);  // Goal 2
  tft.drawRect(80, 71, 1, 59, FIELD);   // Center line
  tft.drawRect(80, 0, 1, 59, FIELD);    // Center line
  tft.drawCircle(80, 65, 6, FIELD);     // Center circle
}
