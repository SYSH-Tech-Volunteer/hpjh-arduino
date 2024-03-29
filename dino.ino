#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const byte BUTTON=2,DINO_HEIGHT=20,DINO_WIDTH=17,TREE_HEIGHT=10,TREE_WIDTH=10,PLAY_HEIGHT=10,PLAY_WIDTH=10,JUMP_MOVE=50,JUMP_HEIGHT=15;
const bool DINO[DINO_HEIGHT][DINO_WIDTH]={
  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1},
  {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
  {0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,0,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0},
};
bool jump,pressed;
byte i,j,x,y,jumpMove,jumpHeight;
int treeX;
void dino(){//畫恐龍
  for(i=0;i<DINO_HEIGHT;i++)for(j=0;j<DINO_WIDTH;j++)display.drawPixel(j,y+i,DINO[i][j]);
}
void setup(){
  Serial.begin(9600);
  pinMode(BUTTON,INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.display();
  delay(1000);
  display.clearDisplay();
}
void loop(){
  display.fillRect(SCREEN_WIDTH-PLAY_WIDTH>>1,SCREEN_HEIGHT-PLAY_HEIGHT>>1,PLAY_WIDTH,PLAY_HEIGHT,1);
  display.fillTriangle(
    (SCREEN_WIDTH>>1)-(PLAY_WIDTH>>2),(SCREEN_HEIGHT>>1)-(PLAY_HEIGHT>>2),
    (SCREEN_WIDTH>>1)+(PLAY_WIDTH>>2),SCREEN_HEIGHT>>1,
    (SCREEN_WIDTH>>1)-(PLAY_WIDTH>>2),(SCREEN_HEIGHT>>1)+(PLAY_HEIGHT>>2),
    0//畫開始符號
  );
  display.display();
  jump=0;
  pressed=1;
  treeX=SCREEN_WIDTH-TREE_WIDTH;
  jumpMove=0;
  jumpHeight=0;
  while(digitalRead(BUTTON));
  while(!digitalRead(BUTTON));
  display.fillRect(SCREEN_WIDTH-PLAY_WIDTH>>1,SCREEN_HEIGHT-PLAY_HEIGHT>>1,PLAY_WIDTH,PLAY_HEIGHT,0);
  display.fillRect(0,SCREEN_HEIGHT-DINO_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT,0);
  y=SCREEN_HEIGHT-DINO_HEIGHT;
  dino();
  display.display();
  while(jump||treeX>DINO_WIDTH){
    if(treeX==SCREEN_WIDTH-TREE_WIDTH)display.fillRect(treeX,SCREEN_HEIGHT-TREE_HEIGHT,TREE_WIDTH,TREE_HEIGHT,0);
    else{
      display.fillRect(treeX,SCREEN_HEIGHT-TREE_HEIGHT,1,TREE_HEIGHT,1);
      display.fillRect(treeX+10,SCREEN_HEIGHT-TREE_HEIGHT,1,TREE_HEIGHT,0);
    }//樹的移動
    display.display();
    if(!digitalRead(BUTTON)&&!jump&&!pressed){
      jumpMove=0;
      jump=pressed=1;
    }
    if(jumpMove<15&&jump){
      display.fillRect(0,SCREEN_HEIGHT-DINO_HEIGHT,DINO_WIDTH,DINO_HEIGHT,0);
      y=SCREEN_HEIGHT-DINO_HEIGHT-jumpHeight;
      dino();
      display.display();
      jumpHeight++;//恐龍上升
    }
    if(jumpMove>35&&jump){
      display.fillRect(0,SCREEN_HEIGHT-DINO_HEIGHT-jumpHeight-1,DINO_WIDTH,DINO_HEIGHT,0);
      y=SCREEN_HEIGHT-DINO_HEIGHT-jumpHeight;
      dino();
      display.display();
      jumpHeight--;//恐龍下降
    }
    if(jumpMove>=JUMP_MOVE){
      jump=0;
      display.fillRect(0,SCREEN_HEIGHT-DINO_HEIGHT-JUMP_HEIGHT,DINO_WIDTH,DINO_HEIGHT,0);
      y=SCREEN_HEIGHT-DINO_HEIGHT;
      dino();
      display.display();//恐龍落地
    }
    if(digitalRead(BUTTON))pressed=0;
    display.display();
    if(treeX>=-DINO_WIDTH)treeX-=1;
    else treeX=SCREEN_WIDTH-TREE_WIDTH;
    if(jump)jumpMove++;//樹回到最右邊
  }
}
