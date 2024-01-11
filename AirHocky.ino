#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Ball Size
const byte VRX1=A0,VRY1=A1,VRX2=A2,VRY2=A3,R=4;
byte score1,score2,ballX,ballY,lastBallX,lastBallY,x1,y1,x2,y2,lx1,lx2,ly1,ly2;
int delX,delY;
void setup(){
  pinMode(VRX1,INPUT);
  pinMode(VRY1,INPUT);
  pinMode(VRX2,INPUT);
  pinMode(VRY2,INPUT);
  pinMode(2,OUTPUT);//搖桿1的5V
  pinMode(4,OUTPUT);//搖桿2的5V
  pinMode(7,OUTPUT);//搖桿2的GND
  digitalWrite(2,1);
  digitalWrite(4,1);
  digitalWrite(7,0);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.display();//
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(1); // Draw white text
}
void loop(){
  score1=score2=0;
  ballX=SCREEN_WIDTH>>1;
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("move ahead");
  display.display();
  while(analogRead(VRY1)<800|| analogRead(VRY2)<800);
  while(score1<3&&score2<3){
    delX=delY=0;
    x1=lx1=SCREEN_WIDTH>>2;
    x2=lx2=(SCREEN_WIDTH>>2)+(SCREEN_WIDTH>>1);
    y1=y2=ly1=ly2=SCREEN_HEIGHT>>1;
    lastBallX=ballX;
    ballY=lastBallY=SCREEN_HEIGHT>>1;
    display.clearDisplay();
    display.display();
    display.fillCircle(x1,y1,R,1);
    display.fillCircle(x2,y2,R,1);
    display.fillCircle(ballX,ballY,R,1);
    while(ballX>=R&&ballX<=SCREEN_WIDTH-R||ballY<=18||ballY>46){  // Ball in the area
      area();
      display.display();
      x1=analogRead(VRX1)*64.0/1023;  // Locate Joystick 1
      y1=analogRead(VRY1)*64.0/1023;
      if(byte(lx1)!=byte(x1)||byte(ly1)!=byte(y1)){
        display.fillCircle(lx1,ly1,R,0);
        area();
        display.fillCircle(x1,y1,R,1);
        display.display();
        lx1=x1;
        ly1=y1;
      }
      x2=analogRead(VRX2)*64.0/1023+64;  // Locate Joystick 2
      y2=analogRead(VRY2)*64.0/1023;
      if(byte(lx2)!=byte(x2)||byte(ly2)!=byte(y2)){
        display.fillCircle(lx2,ly2,R,0);
        area();
        display.fillCircle(x2,y2,R,1);
        display.display();
        lx2=x2;
        ly2=y2;
      }
      if((ballX-x1)*(ballX-x1)+(ballY-y1)*(ballY-y1)<=(R<<1)*(R<<1)){  // PlayeR 1 touch the ball
        delX=(ballX-x1);
        delY=(ballY-y1);
      }
      if((ballX-x2)*(ballX-x2)+(ballY-y2)*(ballY-y2)<=(R<<1)*(R<<1)){  // PlayeR 2 touch the ball
        delX=(ballX-x2);
        delY=(ballY-y2);
      }
      if(delX||delY){  // Ball move
        display.fillCircle(ballX,ballY,R,0);
        lastBallX=ballX;
        lastBallY=ballY;
        ballX+=delX;
        ballY+=delY;
        display.fillCircle(ballX,ballY,R,1);
        display.display();
      }
      if(ballX<R)delX=abs(delX);  // Ball touch the left, delX always positive
      if(ballX>SCREEN_WIDTH-R)delX=-abs(delX);  // Ball touch the Right, delX always negative
      if(ballY<R)delY=abs(delY);  // Ball touch the up, delY always positive
      if(ballY>SCREEN_HEIGHT-R)delY=-abs(delY);  // Ball touch the down, delY always negative
      display.display();
    }
    display.clearDisplay();
    display.setCursor(0,0);
    if(ballX>SCREEN_WIDTH>>1){  // PlayeR 1 score
      score1++;
      ballX=(SCREEN_WIDTH>>1)+(SCREEN_WIDTH>>3);
      display.print(1);
    }
    else{  // PlayeR 2 score
      score2++;
      ballX=(SCREEN_WIDTH>>1)-(SCREEN_WIDTH>>3);
      display.print(2);
    }
    // Score animation
    display.println(" scored");
    display.print("1:");
    display.print(score1);
    display.println("/3");
    display.print("2:");
    display.print(score2);
    display.println("/3");
    display.display();
    delay(1000);
  }
  display.setCursor(0,0);
  display.clearDisplay();
  if(score1>2)display.print(1);
  else display.print(2);
  display.print(" won");
  display.display();
  delay(2000);
}
void area(){
  display.drawLine(0,20,0,44,1);    // Goal 1
  display.drawLine(127,20,127,44,1);  // Goal 2
  display.drawLine(64,0,64,29,1);   // CenteR line
  display.drawLine(64,35,64,64,1);    // CenteR line
  display.drawCircle(64,32,R,1);     // CenteR ciRcle
}
