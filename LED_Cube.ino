int ledPins[] = {2, 3, 4, 8, 12, 13, A1, A2, A3, A5, 5, 6, 9, 10, 11, 7, A0, A4 }; // an array of pin numbers to which LEDs are attached
int pinCount = 18; //Total number of pins to setup

/*Each decoder has an A, B, and C pin
Setup an array for each pin on each decoder.
First decoder is [0] followed by [1], etc.
*/
const int DecoderPinA[3] = {2,8,A1};
const int DecoderPinB[3] = {3,12,A2};
const int DecoderPinC[3] = {4,13,A3};

//Each decoder has a pin to enable it.  Setting this pin to LOW will disable all decoder output
const int EnableDecoder[3] = {7,A0,A4};

//Last column is not connected to line decoder, but directly to an Arduino pin and transistor
const int col25 = A5;

//Each layer is directly connected to Arduino
const int Layer[5] = {5,6,9,10,11};

const int layersTotal = 5; //Total number of layers in cube
const int columnsTotal = 25; //Total number of columns in a layer 5x5

int cubeLayout[5][5]; //A variable for keeping track of what LEDs to light up when working with patterns and designs.  

//Function Prototypes to allow for default argument values
//Must be above the Setup line? http://forum.arduino.cc/index.php/topic,40799.0.html
void led(int targetLed, int brightness=255); 
void SetLayer(int targetLayer, String desiredStatus="on", int brightness=255);
void FadeLed(int targetLed, int runTime=1500, int minBrightness=0, int maxBrightness=255);
void FadeLayer(int targetLayer, int runTime=1500, int minBrightness=0, int maxBrightness=255);
void DesignDiagonalFillV3 (unsigned long RunTime=1000, uint8_t width=1); 
void TransformCube(uint8_t transformDirection, uint8_t wrap=0);
void DesignMiniCubeDance (unsigned long RunTime=5000, int Interval=250);


void setup() {
  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);   
    digitalWrite(ledPins[thisPin], LOW);  
  }
  randomSeed(analogRead(1));
  Serial.begin(9600);
  Serial.println("Setup Done");
}



void loop() {
  //Main loop for various patterns
  int pattern = random(23);
  //Remark out the line below to run random patterns, or set the value to the pattern you want to display
  pattern=20;
  Serial.println(freeRam()); 
  switch(pattern) {
    case 0: 
      LayerWalk(25);
      break;
     case 1: 
       WalkLayerUpDown(25);
       break;
     case 2:
       WalkLayerUpDownReverse(25);
       break;
     case 3:
       WalkSpiral(50);
       break;
     case 4:
       for (int x=0; x <= 30; x++) RandomLed(150); //30 random LEDs
       break;
     case 5:
       for (int x=0; x <= 0; x++) ColumnUpDown(random(25), 150); //Random up and down the column
       break;
     case 6:
       for (int x=0; x <= 0; x++) ColumnUp(random(25), 150); //Random up the column
       break;
     case 7:
       for (int x=0; x <= 0; x++) ColumnDown(random(100, 125), 150); //Random down the column
       break;
     case 8:
       CrawlFullCube(1000);
       break;
     case 9:
       FillFullCube(2000);
       break;
     case 10:
       for (int x=0; x<=5; x++) FadeLed(random(125));
       break;
     case 11:
       for (int x=0; x<20; x++) LayerWalk(1);
       break;
     case 12:
       //LightFullCube(3000);
       break;
     case 13:
       FullReset();
       delay(100);
       DesignExplode(250);
       delay(500);
       break;
     case 14:
       DesignPerim(5000);
       break;
     case 15:
       for (int x=0; x<2500; x++) RandomLed(1);
       break;
     case 16:
       DesignFirework(2000);
       break;
     case 17:
       DesignCheckerboardV2(3000);
       break;
     case 18:
       for (int x=0; x<=5; x++) DesignDiagonalFillV3(1000, random(1, 14));
       break;
     case 19:
       DesignMiniCubeDance(7000, 75);
       break;
     case 20:
       DesignWord("LED CUBE 5x5", 750);
       break;
     case 21:
       DesignSwipes(5000);
       break;
     case 22:
       DesignPyramidCrash(3000);
       break;
     case 23:
       DesignMoveTest(3000);
       break;
     case 24:

       break;
  }
}

void DesignPyramidCrash (int RunTime) {
  //Shows a design of a inverse pyramid and then lower it down
  ClearCube();
  for (uint8_t x=0; x<=4; x++) {
    cubeLayout[4][x]=B11111;
    cubeLayout[0][x]=B11111;
  }
  for (uint8_t x=1; x<=3; x++) {
    cubeLayout[3][x]=B01110;
    cubeLayout[1][x]=B01110;
  }
  cubeLayout[2][2]=B00100;
  ShowDesignV2(cubeLayout, RunTime/6*5);
  for (uint8_t x=0; x<=5; x++) {
    TransformCube(2);
    ShowDesignV2(cubeLayout, RunTime/6);
  }
  
}

void DesignSwipes(int RunTime) {
  //Does a single plane swipe back and forth using shifts
  uint8_t steps=45;
  
  //Shift plane up and down 
  ClearCube();
  for(uint8_t x=0; x<=4; x++) cubeLayout[0][x]=B11111;
  ShowDesignV2(cubeLayout, RunTime/steps); //Show initial plane
  for(uint8_t x=0; x<=3; x++) {
    //Shift plane up
    TransformCube(1);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  for(int x=3; x>=0; x--) {
    //Shift plane down
    TransformCube(2);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  
  //Shift plane forward and backward
  ClearCube();
  for(uint8_t x=0; x<=4; x++) cubeLayout[x][4]=B11111;
  ShowDesignV2(cubeLayout, RunTime/steps); //Show initial plane
  for(uint8_t x=0; x<=3; x++) {
    //Shift forward
    TransformCube(5);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  for(int x=3; x>=0; x--) {
    //Shift backward
    TransformCube(6);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  
  //Shift plane right and left
  ClearCube();
  for(uint8_t x=0; x<=4; x++) 
    for (uint8_t y=0; y<=4; y++) cubeLayout[x][y]=B10000;
  ShowDesignV2(cubeLayout, RunTime/steps); //Show initial plane
  for(uint8_t x=0; x<=3; x++) {
    //Shift right
    TransformCube(4);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  for(int x=3; x>=0; x--) {
    //Shift left
    TransformCube(3);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  
  //Shift plane down and up
  ClearCube();
  for(uint8_t x=0; x<=4; x++) cubeLayout[4][x]=B11111;
  ShowDesignV2(cubeLayout, RunTime/steps); //Show initial plane
  for(int x=3; x>=0; x--) {
    TransformCube(2);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  for(uint8_t x=0; x<=3; x++) {
    TransformCube(1);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  
  //Shift plane backward and forward
  ClearCube();
  for(uint8_t x=0; x<=4; x++) cubeLayout[x][0]=B11111;
  ShowDesignV2(cubeLayout, RunTime/steps); //Show initial plane
  for(int x=3; x>=0; x--) {
    TransformCube(6);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  for(uint8_t x=0; x<=3; x++) {
    TransformCube(5);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }

  //Shift plane left and right
  ClearCube();
  for(uint8_t x=0; x<=4; x++) 
    for (uint8_t y=0; y<=4; y++) cubeLayout[x][y]=B00001;
  ShowDesignV2(cubeLayout, RunTime/steps); //Show initial plane
  for(int x=3; x>=0; x--) {
    TransformCube(3);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
  for(uint8_t x=0; x<=3; x++) {
    TransformCube(4);
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
}
  
  
  
void DesignWord (char displayWord[], int LetterTime) {
  ClearCube(); //Clear the cube before starting
  uint8_t letterCount = strlen(displayWord); //Count the number of letters in the array
  for (uint8_t pass=0; pass<=letterCount; pass++) {  
    DesignLetter(displayWord[pass]);  //Set the front of the cube to the
    for (uint8_t slide=0; slide <=4; slide++) {
      if (slide==0) ShowDesignV2(cubeLayout, LetterTime/4*3); //Leave the front up for longer (3/4 of the time)
      else ShowDesignV2(cubeLayout, LetterTime/4/4); //When sliding back, do it much quicker (1/4 of the remaining 1/4 of the time)
      TransformCube(5); //Slide design to the back 
    }
  }
}


void DesignLetter (char letter) {
  //Accept a single letter and display on front of cube. Could do additional work to place other locations
  //Font: http://fontstruct.com/fontstructions/show/pixel_roughly_5x4
  switch(letter) {
    case 'A':
      cubeLayout[4][4]=B01100;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B11110;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B10010;
      break;
    case 'B':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B11100;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B11110;
      break;
    case 'C':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B10000;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B11110;
      break;
    case 'D':
      cubeLayout[4][4]=B11100;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B10010;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B11100;
      break;
    case 'E':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B11110;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B11110;
      break;
    case 'F':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B11100;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B10000;
      break;
    case 'G':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B10110;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B11110;
      break;
    case 'H':
      cubeLayout[4][4]=B10010;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B11110;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B10010;
      break;
    case 'I':
      cubeLayout[4][4]=B11100;
      cubeLayout[3][4]=B01000;
      cubeLayout[2][4]=B01000;
      cubeLayout[1][4]=B01000;
      cubeLayout[0][4]=B11100;
      break;
    case 'J':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B00100;
      cubeLayout[2][4]=B00100;
      cubeLayout[1][4]=B10100;
      cubeLayout[0][4]=B11100;
      break;
    case 'K':
      cubeLayout[4][4]=B10010;
      cubeLayout[3][4]=B10100;
      cubeLayout[2][4]=B11000;
      cubeLayout[1][4]=B10100;
      cubeLayout[0][4]=B10010;
      break;
    case 'L':
      cubeLayout[4][4]=B10000;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B10000;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B11110;
      break;
    case 'M': //Alternate font 5x5 for readability
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B11011;
      cubeLayout[2][4]=B10101;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B10001;
      break;
    case 'N':
      cubeLayout[4][4]=B10010;
      cubeLayout[3][4]=B11010;
      cubeLayout[2][4]=B10110;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B10010;
      break;
    case 'O':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B10010;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B11110;
      break;
    case 'P':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B11110;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B10000;
      break;
    case 'Q':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B10010;
      cubeLayout[1][4]=B10110;
      cubeLayout[0][4]=B11111;
      break;
    case 'R':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B11110;
      cubeLayout[1][4]=B10100;
      cubeLayout[0][4]=B10010;
      break;
    case 'S':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B11110;
      cubeLayout[1][4]=B00010;
      cubeLayout[0][4]=B11110;
      break;
    case 'T':
      cubeLayout[4][4]=B11100;
      cubeLayout[3][4]=B01000;
      cubeLayout[2][4]=B01000;
      cubeLayout[1][4]=B01000;
      cubeLayout[0][4]=B01000;
      break;
    case 'U':
      cubeLayout[4][4]=B10010;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B10010;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B11110;
      break;
    case 'V':
      cubeLayout[4][4]=B10010;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B10010;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B01100;
      break;
    case 'W': //Alternate font 5x5 for readability
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B10001;
      cubeLayout[1][4]=B10101;
      cubeLayout[0][4]=B11011;
      break;
    case 'X':
      cubeLayout[4][4]=B10010;
      cubeLayout[3][4]=B10010;
      cubeLayout[2][4]=B01100;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B10010;
      break;
    case 'Y':
      cubeLayout[4][4]=B10100;
      cubeLayout[3][4]=B10100;
      cubeLayout[2][4]=B01000;
      cubeLayout[1][4]=B01000;
      cubeLayout[0][4]=B01000;
      break;
    case 'Z':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B00100;
      cubeLayout[2][4]=B01000;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B11110;
      break;
    case ' ':
      ClearCube();
      break;
    case '0':
      cubeLayout[4][4]=B00110;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B01001;
      cubeLayout[1][4]=B01001;
      cubeLayout[0][4]=B00110;
      break;
    case '1':
      cubeLayout[4][4]=B00110;
      cubeLayout[3][4]=B00010;
      cubeLayout[2][4]=B00010;
      cubeLayout[1][4]=B00010;
      cubeLayout[0][4]=B00010;
      break; 
    case '2':
      cubeLayout[4][4]=B00110;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B00010;
      cubeLayout[1][4]=B00100;
      cubeLayout[0][4]=B01111;
      break;
    case '3':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B00001;
      cubeLayout[2][4]=B00111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B01111;
      break;
    case '4':
      cubeLayout[4][4]=B01001;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B01111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B00001;
      break;
    case '5':
      cubeLayout[4][4]=B00111;
      cubeLayout[3][4]=B01000;
      cubeLayout[2][4]=B01111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B01111;
      break;
    case '6':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B01000;
      cubeLayout[2][4]=B01111;
      cubeLayout[1][4]=B01001;
      cubeLayout[0][4]=B01111;
      break;
    case '7':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B00001;
      cubeLayout[2][4]=B00010;
      cubeLayout[1][4]=B00100;
      cubeLayout[0][4]=B01000;
      break;
    case '8':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B00110;
      cubeLayout[1][4]=B01001;
      cubeLayout[0][4]=B01111;
      break;
    case '9':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B01111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B01111;
      break;
    case 'x':
      cubeLayout[4][4]=B00000;
      cubeLayout[3][4]=B00000;
      cubeLayout[2][4]=B00101;
      cubeLayout[1][4]=B00010;
      cubeLayout[0][4]=B00101;
  }  
}




void DesignLetterV1 (char letter) {
  //Accept a single letter and display on front of cube. Could do additional work to place other locations
  //Font: http://www.dafont.com/5x5-pixel.font
  //Alternative: http://www.dafont.com/pixelzim3x5.font
  //http://fontstruct.com/fontstructions/show/pixel_roughly_5x4
  switch(letter) {
    case 'A':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B11111;
      cubeLayout[1][4]=B10001;
      cubeLayout[0][4]=B10001;
      break;
    case 'B':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B11111;
      cubeLayout[1][4]=B10001;
      cubeLayout[0][4]=B11110;
      break;
    case 'C':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B10000;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B11111;
      break;
    case 'D':
      cubeLayout[4][4]=B11110;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B10001;
      cubeLayout[1][4]=B10001;
      cubeLayout[0][4]=B11110;
      break;
    case 'E':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B11111;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B11111;
      break;
    case 'F':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B11110;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B10000;
      break;
    case 'G':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B10111;
      cubeLayout[1][4]=B10001;
      cubeLayout[0][4]=B11111;
      break;
    case 'H':
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B11111;
      cubeLayout[1][4]=B10001;
      cubeLayout[0][4]=B10001;
      break;
    case 'I':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B00100;
      cubeLayout[2][4]=B00100;
      cubeLayout[1][4]=B00100;
      cubeLayout[0][4]=B11111;
      break;
    case 'J':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B00010;
      cubeLayout[2][4]=B00010;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B11110;
      break;
    case 'K':
      cubeLayout[4][4]=B10010;
      cubeLayout[3][4]=B10100;
      cubeLayout[2][4]=B11000;
      cubeLayout[1][4]=B10100;
      cubeLayout[0][4]=B10010;
      break;
    case 'L':
      cubeLayout[4][4]=B10000;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B10000;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B11111;
      break;
    case 'M':
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B11011;
      cubeLayout[2][4]=B10101;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B10001;
      break;
    case 'N':
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B11001;
      cubeLayout[2][4]=B10101;
      cubeLayout[1][4]=B10011;
      cubeLayout[0][4]=B10001;
      break;
    case 'O':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B10001;
      cubeLayout[1][4]=B10001;
      cubeLayout[0][4]=B11111;
      break;
    case 'P':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B11111;
      cubeLayout[1][4]=B10000;
      cubeLayout[0][4]=B10000;
      break;
    case 'Q':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B10001;
      cubeLayout[1][4]=B10011;
      cubeLayout[0][4]=B11111;
      break;
    case 'R':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B11111;
      cubeLayout[1][4]=B10010;
      cubeLayout[0][4]=B10001;
      break;
    case 'S':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B10000;
      cubeLayout[2][4]=B11111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B11111;
      break;
    case 'T':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B00100;
      cubeLayout[2][4]=B00100;
      cubeLayout[1][4]=B00100;
      cubeLayout[0][4]=B00100;
      break;
    case 'U':
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B10001;
      cubeLayout[1][4]=B10001;
      cubeLayout[0][4]=B11111;
      break;
    case 'V':
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B01010;
      cubeLayout[1][4]=B01010;
      cubeLayout[0][4]=B00100;
      break;
    case 'W':
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B10001;
      cubeLayout[2][4]=B10001;
      cubeLayout[1][4]=B10101;
      cubeLayout[0][4]=B11011;
      break;
    case 'X':
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B01010;
      cubeLayout[2][4]=B00100;
      cubeLayout[1][4]=B01010;
      cubeLayout[0][4]=B10001;
      break;
    case 'Y':
      cubeLayout[4][4]=B10001;
      cubeLayout[3][4]=B01010;
      cubeLayout[2][4]=B00100;
      cubeLayout[1][4]=B00100;
      cubeLayout[0][4]=B00100;
      break;
    case 'Z':
      cubeLayout[4][4]=B11111;
      cubeLayout[3][4]=B00010;
      cubeLayout[2][4]=B00100;
      cubeLayout[1][4]=B01000;
      cubeLayout[0][4]=B11111;
      break;
    case ' ':
      ClearCube();
      break;
    case '0':
      cubeLayout[4][4]=B00110;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B01001;
      cubeLayout[1][4]=B01001;
      cubeLayout[0][4]=B00110;
      break;
    case '1':
      cubeLayout[4][4]=B00110;
      cubeLayout[3][4]=B00010;
      cubeLayout[2][4]=B00010;
      cubeLayout[1][4]=B00010;
      cubeLayout[0][4]=B00010;
      break; 
    case '2':
      cubeLayout[4][4]=B00110;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B00010;
      cubeLayout[1][4]=B00100;
      cubeLayout[0][4]=B01111;
      break;
    case '3':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B00001;
      cubeLayout[2][4]=B00111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B01111;
      break;
    case '4':
      cubeLayout[4][4]=B01001;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B01111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B00001;
      break;
    case '5':
      cubeLayout[4][4]=B00111;
      cubeLayout[3][4]=B01000;
      cubeLayout[2][4]=B01111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B01111;
      break;
    case '6':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B01000;
      cubeLayout[2][4]=B01111;
      cubeLayout[1][4]=B01001;
      cubeLayout[0][4]=B01111;
      break;
    case '7':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B00001;
      cubeLayout[2][4]=B00010;
      cubeLayout[1][4]=B00100;
      cubeLayout[0][4]=B01000;
      break;
    case '8':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B00110;
      cubeLayout[1][4]=B01001;
      cubeLayout[0][4]=B01111;
      break;
    case '9':
      cubeLayout[4][4]=B01111;
      cubeLayout[3][4]=B01001;
      cubeLayout[2][4]=B01111;
      cubeLayout[1][4]=B00001;
      cubeLayout[0][4]=B01111;
      break;
  }  
}


void DesignMiniCubeDance (unsigned long RunTime, int Interval) {
  uint8_t steps = RunTime/Interval;
  uint8_t previousMove;
  ClearCube();
  //Start with a cube in the middle
  cubeLayout[2][2]=B00110;
  cubeLayout[2][3]=B00110;
  cubeLayout[3][2]=B00110;
  cubeLayout[3][3]=B00110;
  ShowDesignV2(cubeLayout, RunTime/steps);
  for (uint8_t pass=0; pass<=steps;) {    
    uint8_t moveDirection=random(1,7); //Random direction to move the cube

    for (uint8_t layer=0; layer<=4; layer++) { //Checking to see if already on the north or south side
        if (moveDirection==5 && cubeLayout[layer][0]!=0) moveDirection=0; //If against North side, do not move North
        if (moveDirection==6 && cubeLayout[layer][4]!=0) moveDirection=0; //If against South side, do not move South
      }
      
    for (uint8_t row=0; row<=4; row++) { //Checking to see if already on Top or Bottom side
      if (moveDirection==1 && cubeLayout[4][row]!=0) moveDirection=0; //If already on top side, do not move up
      if (moveDirection==2 && cubeLayout[0][row]!=0) moveDirection=0; //If already on bottom side, do not down up
      }
      
    for (uint8_t layer=0; layer<=4; layer++) { //Checking to see if already on east or west side
      for (uint8_t row=0; row<=4; row++) {
        if (moveDirection==3 && bitRead(cubeLayout[layer][row], 4)!=0) moveDirection=0; //If already on West side, do not move West
        if (moveDirection==4 && bitRead(cubeLayout[layer][row], 0)!=0) moveDirection=0; //If already on East side, do not move East
      }
    }
    if ((moveDirection==1 && previousMove==2) || (moveDirection==2 && previousMove==1)) moveDirection=0; //Keep it from jittering back and forth
    if ((moveDirection==3 && previousMove==4) || (moveDirection==4 && previousMove==3)) moveDirection=0;
    if ((moveDirection==5 && previousMove==6) || (moveDirection==6 && previousMove==5)) moveDirection=0;
    
    if (moveDirection!=0) { //If direction to move isn't invalid, then move
      TransformCube(moveDirection);
      ShowDesignV2(cubeLayout, RunTime/steps);
      previousMove=moveDirection; //Keep track of the previous move direction
      pass++;
    }
    

  }
}


void DesignMoveTest (unsigned long RunTime) {
  uint8_t steps=16;
  ClearCube();
  cubeLayout[2][2]=B00110;
  cubeLayout[2][3]=B00110;
  cubeLayout[3][2]=B00110;
  cubeLayout[3][3]=B00110;
  
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(1);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(2);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(2);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(5);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(5);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(5,1);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(5,1);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(5);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(5);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(6);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(6);
  ShowDesignV2(cubeLayout, RunTime/steps); 
    TransformCube(4);
  ShowDesignV2(cubeLayout, RunTime/steps); 

  TransformCube(4,1);
  ShowDesignV2(cubeLayout, RunTime/steps); 
    TransformCube(4,1);
  ShowDesignV2(cubeLayout, RunTime/steps); 
  TransformCube(4,1);
  ShowDesignV2(cubeLayout, RunTime/steps); 

}

void TransformCube(uint8_t transformDirection, uint8_t wrap) {
  //Wrap 0 does not wrap around and clears any data pushed off
  //Wrap 1 wraps data around the cube
  //Wrap 2 does not wrap around, but does not clear out previous data in last dimension
  uint8_t temp[5]; //Create a temp table to hold the data for wrapping before overwriting
  
  if (transformDirection==1) { //Move the layers up
    if(wrap==1) { //Wrap layer 4 around to Layer 0
      for (uint8_t row=0; row<=4; row++) temp[row]=cubeLayout[4][row]; //Store the values of layer 4 in the temp array
    } 
    for (uint8_t layer=4; layer >=1; layer--) {
    for (uint8_t row=0; row <=4; row++) {
      cubeLayout[layer][row]=cubeLayout[layer-1][row]; //Copy each row in a layer from the previous layer
      }
    }
    if (wrap==1) for (uint8_t row=0; row<=4; row++) cubeLayout[0][row]=temp[row]; //Use the values in temp to populate Layer 0 which used to be 4
    if (wrap==0) for (uint8_t row=0; row<=4; row++) cubeLayout[0][row]=0; //Clear out Layer0
  }
  
  if (transformDirection==2) { //Move the layers down
    if(wrap==1) { //Wrap layer 4 around to Layer 0
      for (uint8_t row=0; row<=4; row++) temp[row]=cubeLayout[0][row]; //Store the values of layer 0 in the temp array
    }
    for (uint8_t layer=0; layer <=3; layer++) {
      for (uint8_t row=0; row <=4; row++) {
        cubeLayout[layer][row]=cubeLayout[layer+1][row]; //Copy each row in a layer from the above layer
      }
    }
    if (wrap==1) for (uint8_t row=0; row<=4; row++) cubeLayout[4][row]=temp[row]; //Use the values in temp to populate Layer 4 which used to be 0
    if (wrap==0) for (uint8_t row=0; row<=4; row++) cubeLayout[4][row]=0; //Clear out Layer4
  }


    if(transformDirection==3) { //Move the cube to the left
      for(int layer=0; layer<=4; ++layer) {
        for(int row=0; row<=4; ++row) {
          uint8_t bits = cubeLayout[layer][row]; //Temp variable for working with bit shifts
	  bits <<= 1; //Shift all bits for this row to the left by one.  Will leave a 0 on the far right and an extra digit outside of the scope on the left
	  if(wrap==1) bits |= bits >> 5; //When wrapping, use the bitwise OR along with an instance of the row shifted to the right by 5 digits, leaving only what used to be the left most bit
          bits &= B11111; //Use bitwise AND along with all 1 bits to limit only the right most 5 bits, dropping off anything that was shifted to the left and would be remaining
	  cubeLayout[layer][row] = bits; //Update the cube data with this updated row that has been shifted to the left
        }
      }
    }
    
    if(transformDirection == 4) { //Move the cube to the right
    //Need to figure out how to leave the right most bit alone for wrap mode 2
      for(int layer=0; layer<=4; ++layer) {
        for(int row=0; row<=4; ++row) {
          uint8_t bits = cubeLayout[layer][row]; //temp variable for working with the bit shifts
          uint8_t temp = bits & 1; //Save the current value of the right most bit to another temp location in the event of wrapping
          bits >>= 1; //Move all bits in the row to the right by one position, dropping off the right most digit, which is saved in tmp
          if(wrap==1) bits |= temp << 4; //If wrapping is desired, then use bitwise OR along with the tmp value shifted to the left 4 positions, putting it in the left most
          cubeLayout[layer][row] = bits; //Update the cube data with this updated row that has been shifted to the right
        }
      }
    }

  if (transformDirection==5) { //Move the cube forward
    if(wrap==1) for (uint8_t layer=0; layer<=4; layer++) temp[layer]=cubeLayout[layer][0]; //Store values of row 0 in the temp array
    for (uint8_t row=0; row<=3; row++) {
      for (uint8_t layer=0; layer<=4; layer++) {
        cubeLayout[layer][row]=cubeLayout[layer][row+1];
      }
    }
    if (wrap==1) for (uint8_t layer=0; layer<=4; layer++) cubeLayout[layer][4]=temp[layer]; //Use the values in temp to populate row 4 which used to be 0
    if (wrap==0) for (uint8_t layer=0; layer<=4; layer++) cubeLayout[layer][4]=0; //Clear out Row 4
  }
  
  if (transformDirection==6) { //Move the cube backwards
    if(wrap==1) for (uint8_t layer=0; layer<=4; layer++) temp[layer]=cubeLayout[layer][4]; //Store values of row 4 in the temp array
    for (uint8_t row=4; row>=1; row--) {
      for (uint8_t layer=0; layer<=4; layer++) {
        cubeLayout[layer][row]=cubeLayout[layer][row-1];
      }
    }
    if (wrap==1) for (uint8_t layer=0; layer<=4; layer++) cubeLayout[layer][0]=temp[layer]; //Use the values in temp to populate row 0 which used to be 4
    if (wrap==0) for (uint8_t layer=0; layer<=4; layer++) cubeLayout[layer][0]=0; //Clear out Row 0
  }
}


void ClearCube () {
  //Sets the cubeLayout variable to all 0
  memset(cubeLayout,0,sizeof(cubeLayout));
}

void FillCube () {
  //Turns on all LEDs for the cube
  for (byte layer=0; layer<=4; layer++) {
    for (byte row=0; row<=4; row++) {
      cubeLayout[layer][row]=31;
    }
  }
}
  

void DesignDiagonalFillV3 (unsigned long RunTime, uint8_t width) {
  //uint8_t steps=13+width+width+3; //Total number of steps to take when 2 swipes
  uint8_t steps=13+width;
  ClearCube(); //Turn off the cube output before adding new pattern
  
  for (uint8_t pass=0; pass<=steps; pass++) {
    //Copy layers up, leaving Layer 0 alone
    TransformCube(1,2);
    /*
    for (uint8_t layer=4; layer >=1; layer--) {
      for (uint8_t row=0; row <=4; row++) {
        cubeLayout[layer][row]=cubeLayout[layer-1][row];
      }
    }
    */

    //On Layer 0, copy rows up, leaving the bottom row alone
    //LED 0 is on Row 4, so leaving it along to introduce changes
    for (uint8_t row=0; row <=3; row++) {
      cubeLayout[0][row]=cubeLayout[0][row+1];
    }
    //For layer 0, row 4, shift the bit to the right by one and then add 16 to fill it back in, eventually filling in the row
    if (pass==0) cubeLayout[0][4]=B10000; //On the first pass, start with LED 0
    else if (pass<width) {
      cubeLayout[0][4]>>=1; //Move the LED to the right
      cubeLayout[0][4]+=16; //Add 16 to fill in the left most bit
    }
    /*
    //Add a second swipe after a 3 frame blank
    else if (pass>(width+2) && pass<(width+width+3)) {
      cubeLayout[0][4]>>=1; //Move the LED to the right
      cubeLayout[0][4]+=16; //Add 16 to fill in the left most bit
    }
    */
    else cubeLayout[0][4]>>=1; //Only shift LED in layer 0 row 4 over, and do not add LED0 again
    //Serial.println("Showing design");
    ShowDesignV2(cubeLayout, RunTime/steps);
  }
}


void ShowDesignV2(int frame[5][5], unsigned long RunTime) {
  //Accepts a 5 array and displays the output simultaneous
  //Output is significantly dimmer than single as we are flashing off and on and limited power per layer

unsigned long ledLine[5];
//Take all layer and row information and put into a long array for each layer to walk through
for (int layer=0; layer<=4; layer++) {
  for (int row=4; row>=0; row--) {
    if (row == 4) ledLine[layer]=frame[layer][4]; //For first entry, set the value to the lowest row, since the decoders are connected starting at the bottom if viewed from above.
    else ledLine[layer] = (ledLine[layer] << 5) | frame[layer][row]; //Shift the existing values to the left by 5 digits, then OR the next row into the empty area
  }
  ledLine[layer] = (ledLine[layer] << 7); //Shift further over by 7 digits to fill out the space for the 4th virtual decoder
}
  /*
  Serial.println(RunTime);
  Serial.println(ledLine[0]);
  Serial.println(ledLine[1]);
  Serial.println(ledLine[2]);
  Serial.println(ledLine[3]);
  Serial.println(ledLine[4]);
  */
  unsigned long StartTime = millis();
  unsigned long CurrentTime = millis();
  //Run through this function for the specified amount of time
  while (CurrentTime - StartTime <= RunTime) {
    /*
    String space = " ";
    String stringOutput=CurrentTime + space + StartTime + space + RunTime;
    Serial.println(stringOutput);
    */
    for (int layer=0;layer<=4;layer++) { //Cycle through each layer
      FullReset(); // Turn off all output to prepare for this layer
      SetLayer(layer, "On");
      //Cycle through each element to see if the decoder output needs to be on, and if so, activate it  
      for (int output=0; output<=7; output++) {
        for (int decoder=0; decoder<=3; decoder++) {
                  /*
                  unsigned long shift=31-(decoder*8)-output;
                  String stringShift = " Shift ";
                  String stringBit = " BitResult ";
                  unsigned long BitResult = ledLine[layer] & (1<<shift);
                  String stringDecoder = " Decoder ";
                  String stringOutput = " Output ";
                  String SerialOutput = stringShift + shift + stringBit + BitResult + stringDecoder + decoder + stringOutput + output;
                  Serial.println(SerialOutput);
                  */
          //if (ledLine[layer] & (1<<31-(decoder*8)-output)) { //Need to shift over 31 digits to start at first position, if working on another decoder, move 8 less digits, and one less per output
          if (ledLine[layer] & (2147483648 >>(decoder*8)+output)) {//Possibly simpler to understand, as the starting point is at LED 0 and then shifting to the right
                  SetDecoder(decoder, output); 
                } else {
                    //digitalWrite(EnableDecoder[decoder], LOW);
                    if (decoder <=2 ) digitalWrite(EnableDecoder[decoder], LOW);
                    if (decoder == 3) digitalWrite(col25, LOW);
            }
        }
      }
    CurrentTime = millis();
    }
    FullReset();
  }
}
  
void DesignFirework(unsigned long RunTime) {
  // shows a column going up, then explosions with random LEDs on 2 layers and fading out
  ColumnUp(12, 150);
  //Randomly flash LED on two layers at a time, fading out each time
  for (int minled=75, maxLed=125, brightness=193; minled>=0; minled=minled-25, maxLed=maxLed-25, brightness=brightness-64) {
    for(int x=0; x<=100; x++) {
      led(random(minled, maxLed), brightness);
      delay(5);
    }
  }
}


void DesignExplode(unsigned long RunTime) {
  //3 Frame animation starting with center LED and exploding out
  int framecount=3;
  boolean frame[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 1, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 3
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 4
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}
    }
  };
  ShowDesign(frame, RunTime/framecount);
   boolean frame2[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {0, 1, 0, 1, 0},
      {0, 0, 1, 0, 0},
      {0, 1, 0, 1, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {0, 0, 1, 0, 0},
      {0, 1, 1, 1, 0},
      {0, 0, 1, 0, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 3
      {0, 0, 0, 0, 0},
      {0, 1, 0, 1, 0},
      {0, 0, 1, 0, 0},
      {0, 1, 0, 1, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 4
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}
    },   
  };
  ShowDesign(frame2, RunTime/framecount);
  boolean frame3[5][5][5] = 
  {
    {//Layer 0
      {1, 0, 0, 0, 1},
      {0, 0, 0, 0, 0},
      {0, 0, 1, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 1}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {0, 1, 0, 1, 0},
      {0, 0, 1, 0, 0},
      {0, 1, 0, 1, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 2
      {0, 0, 1, 0, 0},
      {0, 0, 1, 0, 0},
      {1, 1, 1, 1, 1},
      {0, 0, 1, 0, 0},
      {0, 0, 1, 0, 0}
    },
    {//Layer 3
      {0, 0, 0, 0, 0},
      {0, 1, 0, 1, 0},
      {0, 0, 1, 0, 0},
      {0, 1, 0, 1, 0},
      {0, 0, 0, 0, 0}
    },
    {//Layer 4
      {1, 0, 0, 0, 1},
      {0, 0, 0, 0, 0},
      {0, 0, 1, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 1}
    },   
  };
  ShowDesign(frame3, RunTime/framecount*2);
}


void DesignPerim(unsigned long RunTime) {
  //Design that lights up only the permimeter edges
  boolean frame[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 1, 1, 1},
      {1, 0, 0, 0, 1},
      {1, 0, 0, 0, 1},
      {1, 0, 0, 0, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 0, 0, 0, 1},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 1}
    },
    {//Layer 2
      {1, 0, 0, 0, 1},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 1}
    },
    {//Layer 3
      {1, 0, 0, 0, 1},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 1}
    },
    {//Layer 4
      {1, 1, 1, 1, 1},
      {1, 0, 0, 0, 1},
      {1, 0, 0, 0, 1},
      {1, 0, 0, 0, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame, RunTime);
}

void DesignCheckerboardV2(unsigned long RunTime) {
  //Layer 0
  cubeLayout[0][0]=B10101;
  cubeLayout[0][1]=B01010;
  cubeLayout[0][2]=B10101;
  cubeLayout[0][3]=B01010;
  cubeLayout[0][4]=B10101;
   //Layer 1
  cubeLayout[1][0]=B01010;
  cubeLayout[1][1]=B10101;
  cubeLayout[1][2]=B01010;
  cubeLayout[1][3]=B10101;
  cubeLayout[1][4]=B01010;
    //Layer 2
  cubeLayout[2][0]=B10101;
  cubeLayout[2][1]=B01010;
  cubeLayout[2][2]=B10101;
  cubeLayout[2][3]=B01010;
  cubeLayout[2][4]=B10101;
    //Layer 3
  cubeLayout[3][0]=B01010;
  cubeLayout[3][1]=B10101;
  cubeLayout[3][2]=B01010;
  cubeLayout[3][3]=B10101;
  cubeLayout[3][4]=B01010;
    //Layer 4
  cubeLayout[4][0]=B10101;
  cubeLayout[4][1]=B01010;
  cubeLayout[4][2]=B10101;
  cubeLayout[4][3]=B01010;
  cubeLayout[4][4]=B10101;
  ShowDesignV2(cubeLayout, RunTime);
}


void ShowDesign(boolean frame[5][5][5], unsigned long RunTime) {
  //Accepts a 5x5x5 boolean array and displays the output simultaneous
  //Output is significantly dimmer than single as we are flashing off and on and limited power per layer
  boolean frameDecoderView [5][4][8];
 
  for (int layer=0; layer<=4; layer++) {
    //Move data from human logical frame array to an array of data for each decoder
    //d = DecoderData location, x = Frame array location
    //Move column 0-4 into Decoder 0 array
    for (int DecoderOutput=0; DecoderOutput<=4; DecoderOutput++) frameDecoderView[layer][0][DecoderOutput] = frame[layer][4][DecoderOutput];
    //Move column 5-7 into Decoder 0
    for (int DecoderOutput=5, x=0; x<=2; DecoderOutput++, x++) frameDecoderView[layer][0][DecoderOutput] = frame[layer][3][x];
    //column 8-9 into Decoder 1
    for (int DecoderOutput=0, x=3; x<=4; DecoderOutput++, x++) frameDecoderView[layer][1][DecoderOutput] = frame[layer][3][x];
    //Column 10-14 into Decoder 1
    for (int DecoderOutput=2, x=0; x<=4; DecoderOutput++, x++) frameDecoderView[layer][1][DecoderOutput] = frame[layer][2][x];
    //Column 15 into Decoder 1
    for (int DecoderOutput=7, x=0; x<=0; DecoderOutput++, x++) frameDecoderView[layer][1][DecoderOutput] = frame[layer][1][x];  
    //Column 16-19 into Decoder 2
    for (int DecoderOutput=0, x=1; x<=4; DecoderOutput++, x++) frameDecoderView[layer][2][DecoderOutput] = frame[layer][1][x];
    //Column 20-23 into Decoder 3
    for (int DecoderOutput=4, x=0; x<=3; DecoderOutput++, x++) frameDecoderView[layer][2][DecoderOutput] = frame[layer][0][x];  
    //Column 24 into Decoder 4, or stand-alone pin
    for (int DecoderOutput=0, x=4; x<=4; DecoderOutput++, x++) frameDecoderView[layer][3][DecoderOutput] = frame[layer][0][x];  
    //Fill rest of Decoder 3 with zero
    for (int DecoderOutput=1; DecoderOutput<=7; DecoderOutput++) frameDecoderView[layer][3][DecoderOutput] = 0;
  }

  unsigned long StartTime = millis();
  unsigned long CurrentTime = millis();
  //Run through this function for the specified amount of time
  while (CurrentTime - StartTime <= RunTime) {
    for (int layer=0;layer<=4;layer++) { //Cycle through each layer
      FullReset(); // Turn off all output to prepare for this layer
      SetLayer(layer, "On");
      //Cycle through each element to see if the decoder output needs to be on, and if so, activate it  
      for (int output=0; output<=7; output++) {
        for (int decoder=0; decoder<=3; decoder++) {
          if (frameDecoderView[layer][decoder][output] == 1) SetDecoder(decoder, output);
            else {
              //digitalWrite(EnableDecoder[decoder], LOW);
              if (decoder <=2 ) digitalWrite(EnableDecoder[decoder], LOW);
              if (decoder == 3) digitalWrite(col25, LOW);
            }
        }
      }
    CurrentTime = millis();
    }
    FullReset();
  }
}




void FadeLed(int targetLed, int runTime, int minBrightness, int maxBrightness) {
  //Calculate which layer needs to be used
  //Divide the targetLed by the total number of columns in a layer
  //Since it will round down to the nearest whole number, this gives the layer
  int targetLayer = targetLed / columnsTotal;
  //Subtract the number of columns in other layers
  //Result is the column needed for this layer
  int targetColumn = targetLed - (targetLayer * columnsTotal);
  //Calculate which decoder to address
  //Each line decoder can output to 8 pins each.
  int targetDecoder = targetColumn / 8;
  //Calculate the input for the targetDecoder
  //The decoder will accept this on 3 pins as binary to output to 8 pins
  int targetDecoderInput = targetColumn % 8;
  //Turn off unneeded decoders and layers
  //FullReset();
  for (int x = 0; x<=2; x++) {
    if (x != targetDecoder) 
    //Except for the targetDecoder, turn all others off
    digitalWrite(EnableDecoder[x], LOW);
  }
  for (int x = 0; x<=5; x++) {
    //Turn off other layers not needed
    if (x != targetLayer) digitalWrite(Layer[x], LOW);
  }
  //Set the decoder output to the correct pin
  SetDecoder(targetDecoder, targetDecoderInput);
  //Special handling for column 24 as it is directly connected to Arduino and not a line decoder
  if (targetColumn == 24) digitalWrite(col25, HIGH);
  else digitalWrite(col25, LOW);
  //Enable power to the target layer
  //SetLayer(targetLayer, "On");
  FadeLayer(targetLayer, runTime, minBrightness, maxBrightness);
}



void FadeLayer(int targetLayer, int runTime, int minBrightness, int maxBrightness) {
  //Accepts a Layer, and brightness values, and runs for that long, fading the value in and out
  //Run through this function for the specified amount of time
  int delayTime = runTime / 2 / 51;
  int fadeInterval = (maxBrightness - minBrightness) / 51;
  for(int fadeValue = minBrightness ; fadeValue <= maxBrightness; fadeValue += fadeInterval) { 
    analogWrite(Layer[targetLayer], fadeValue);         
    //Wait to see the dimming effect
    delay(delayTime);
  } 
  // fade out from max to min in increments of 5 points:
  for(int fadeValue = maxBrightness ; fadeValue >= minBrightness; fadeValue -=fadeInterval) { 
    // sets the value (range from 0 to 255):
    analogWrite(Layer[targetLayer], fadeValue);         
    //Wait to see the dimming effect
    delay(delayTime);                            
  } 
}

void CrawlFullCube(int RunTime) {
  //Lights all LEDs on a single layer, crawling up and down the full cube
  FullReset();
  CrawlFullCubeUp(RunTime/2);
  CrawlFullCubeDown(RunTime/2);
}

void FillFullCube(int RunTime) {
  //Lights all LEDs in the cube, filling up the cube and then down
  FullReset();
  FillFullCubeUp(RunTime/2);
  FillFullCubeDown(RunTime/2);
}

void CrawlFullCubeUp (int RunTime) {
  //Lights all LEDs on a single layer at a time, crawling up the cube
  //long StartTime = millis();
  //unsigned long CurrentTime = millis();
  for (int x=0; x<=4; x++) {
    if (x !=0) SetLayer(x-1, "Off");
    SetLayer(x, "On");
    CyclePins(RunTime/5);
  }
}

void CrawlFullCubeDown (int RunTime) {
  //Lights all LEDs on a single layer at a time, crawling down the cube
  for (int x=4; x>=0; x--) {
    if (x !=4) SetLayer(x+1, "Off");
    SetLayer(x, "On");
    CyclePins(RunTime/5);
  }
}


void FillFullCubeDown (int RunTime) {
  //Lights all LED on the entire cube for a duration of milliseconds emptying down the layers
  for (int x=4; x>=0; x--) {
    SetLayer(x, "Off");
    CyclePins(RunTime/5);
  }
}


void FillFullCubeUp (int RunTime) {
  //Lights all LED on the entire cube for a duration of milliseconds filling up the layers
  int CurrentLayer = 0;
  for (int x=0; x<=4; x++) {
    SetLayer(x, "On");
    CyclePins(RunTime/5);
  }
}


void CyclePins(int RunTime) {
  //Cycles through all Decoder pins for a specified amount of milliseconds
  for (int x=0; x<=3; x++) digitalWrite(EnableDecoder[x], HIGH);
  long StartTime = millis();
  unsigned long CurrentTime = millis();
  //Run through this function for the specified amount of time
  while (CurrentTime - StartTime <= RunTime) {
    //Cycle through each Decoder output, at a specified delay
    for (int x=0; x<=7; x++) {
      if (x==0) digitalWrite(col25, HIGH); //To keep the last column in sync, it cycles off an on like the others
      if (x==1) digitalWrite(col25, LOW); //To keep the last column in sync, it cycles off an on like the others
      //digitalWrite(col25, HIGH);
      SetDecoder(0, x);
      SetDecoder(1, x);
      SetDecoder(2, x);
      //digitalWrite(col25, LOW);
      //delayMicroseconds(250); //When using 1 millisecond, the 25th column looks dimmer
    }
    CurrentTime = millis();
  }
}

void LightFullCube (int RunTime) {
  //Lights all LED on the entire cube for a duration of milliseconds
  for (int x=0; x<=4; x++) SetLayer(x, "On"); //Turn all layers on
  //Run through this function for the specified amount of time
  CyclePins(RunTime);
}


void RandomLed (int delayTime) {
  led(random(125));
  delay(delayTime);
}

void ColumnUpDown (int targetLed, int delayTime) {
  //Starts at the bottom layer and lights up the cube, and then starts going back down
  led(targetLed);
  delay(delayTime);
  for (int x=1; x<=4; x++) {
    SetLayer(x, "On");
    delay(delayTime);
  }
  for (int x=4; x>=0; x--) {
    SetLayer(x, "Off");
    delay(delayTime);
  }
}

void ColumnUp (int targetLed, int delayTime) {
  //Accepts a layer 0 led and a delay and crawls up
  FullReset();
  led(targetLed);
  delay(delayTime);
  for (int x=1; x<=4; x++) {
    SetLayer(x, "On");
    delay(delayTime);
  }
  for (int x=0; x<=4; x++) {
    SetLayer(x, "Off");
    delay(delayTime);
  }
}
  
void ColumnDown (int targetLed, int delayTime) {
  //Accepts a layer 5 led and a delay and crawls down
  FullReset();
  led(targetLed);
  delay(delayTime);
  for (int x=3; x>=0; x--) {
    SetLayer(x, "On");
    delay(delayTime);
  }
  for (int x=4; x>=0; x--) {
    SetLayer(x, "Off");
    delay(delayTime);
  }
}


int WalkSpiral(int delayTime) {
  //Starting at 0, spiral around the perimeter spiraling in to the middle then unwind on next layer
  int interval = 5;
  int iteration = 4;
  int i = 0;
  int direction = 1;
  
  for (;i != 125;) {
    //Spiral to inside
    for (int x=0; x <= 2; x++) {
      //South side
      //For number of iterations left, go sequentially forward
      for (int s=0; s <= iteration; s++) {
        led(i);
        delay(delayTime);
        if (s != iteration) i+= direction;
        else {
          //One less interation
          iteration--;
          //Next LED
          i = i + interval;
        }
      }
      //East side
      //For number of iterations left, go up the interval value, following one side
      for (int e=0; e <= iteration; e++) {
        led(i);
        delay(delayTime);
        if (e != iteration) i = i + interval;
        else i-= direction;
      }
      //North side
      //For number of iterations left, go across layer, sequentially lower
      for (int n=0; n <= iteration; n++) {
        led(i);
        delay(delayTime);
        if (n != iteration) i-=direction;
        else {
          //One less iteration
          iteration--;
          //Next LED
          i = i - interval;
        }
      }
      //For the current iteration value, go down the interval value
      // West side
      for (int w=0; w <= iteration; w++) {
        led(i);
        delay(delayTime);
        if (w != iteration) i = i - interval;
        else i+= direction;
      }
    } 
   i = i + 8;
   iteration = 4;
//  direction = -direction;
  }
}

void WalkLayerUpDownReverse(int delayTime) {
  //Rather than walking through LED sequentially, go up and down the columns going down
  int interval = -5;
  int i = 124;
  int direction = -1;
  //Start at 124, and walk through every LED in reverse
  while (i >= 0 ) {
    //Along the X axis
    for (int x=0; x <= 4; x++) {
      //Along the Y axis
      for (int y=0; y <= 4; y++) {
        led(i);
        delay(delayTime);
        //Not incrementing if last time through
        if (y != 4) i = i + interval;  
      }
      //When at the end of a row of columns, inverse the interval to go backwards on next row of columns
      //Change direction of the interval
      interval = -interval;
      //Do not increment if last time through
      if (x != 4) i = i + direction;
    }
    //Jump up a layer 
    i = i - 25;
    //Change direction after a layer is completed
    direction = -direction;
  }
}

void WalkLayerUpDown(int delayTime) {
  //Rather than walking through LED sequentially, go up and down the columns going up
  int interval = 5;
  int i = 0;
  int direction = 1;
  //Start at 0, and walk through every LED
  while (i < layersTotal * columnsTotal) {
    //Along the X axis
    for (int x=0; x <= 4; x++) {
      //Along the Y axis
      for (int y=0; y <= 4; y++) {
        led(i);
        delay(delayTime);
        //Not incrementing if last time through
        if (y != 4) i = i + interval;  
      }
      //When at the end of a row of columns, inverse the interval to go backwards on next row of columns
      //Change direction of the interval
      interval = -interval;
      //Do not increment if last time through
      if (x != 4) i = i + direction;
    }
    //Jump up a layer 
    i = i + 25;
    //Change direction after a layer is completed
    direction = -direction;
  }
}

void Status() {
  int layerstatus;
  for (int i=0; i<3; i++) {
    String line1 = "Decoder ";
    String line2 = " Status Pins CBA:";
    int decoder0a = (digitalRead(DecoderPinC[i]));
    int decoder0b = (digitalRead(DecoderPinB[i]));
    int decoder0c = (digitalRead(DecoderPinA[i]));
    String enabled = " Enabled: ";
    int decoder0enabled = (digitalRead(EnableDecoder[i]));
    String x = line1 + i + line2 + decoder0a + decoder0b + decoder0c + enabled + decoder0enabled;
    Serial.println(x);
  }
  int col25Status = (digitalRead(col25));
  String line = "Column 25: ";
  String x = line + col25Status;
  Serial.println(x);
  String line1 = "Layers: ";
  int layerStatus[5];
  for (int i=0; i<5; i++) {
    layerStatus[i] = (digitalRead(Layer[i]));
    //String line1 = line1 + layerstatus;    
  }
  x = line1 + layerStatus[0] + layerStatus[1] + layerStatus[2] + layerStatus[3] + layerStatus[4];
  Serial.println(x);
  //Need to add entries for column 25 as well as layers
}
 
void led(int targetLed, int brightness) {
  //Calculate which layer needs to be used
  //Divide the targetLed by the total number of columns in a layer
  //Since it will round down to the nearest whole number, this gives the layer
  int targetLayer = targetLed / columnsTotal;
  //Subtract the number of columns in other layers
  //Result is the column needed for this layer
  int targetColumn = targetLed - (targetLayer * columnsTotal);
  //Calculate which decoder to address
  //Each line decoder can output to 8 pins each.
  int targetDecoder = targetColumn / 8;
  //Calculate the input for the targetDecoder
  //The decoder will accept this on 3 pins as binary to output to 8 pins
  int targetDecoderInput = targetColumn % 8;
  //Turn off unneeded decoders and layers
//  FullReset();
  
  for (int x = 0; x<=2; x++) {
    if (x != targetDecoder) 
    //Except for the targetDecoder, turn all others off
    digitalWrite(EnableDecoder[x], LOW);
  }
  for (int x = 0; x<=5; x++) {
    //Turn off other layers not needed
    if (x != targetLayer) digitalWrite(Layer[x], LOW);
  }
  
  //Set the decoder output to the correct pin
  SetDecoder(targetDecoder, targetDecoderInput);
  //Special handling for column 24 as it is directly connected to Arduino and not a line decoder
  if (targetColumn == 24) digitalWrite(col25, HIGH);
  else digitalWrite(col25, LOW);
  //Enable power to the target layer
  SetLayer(targetLayer, "On", brightness);
}
 
void SetDecoder (int targetDecoder, int targetOutput) {
  //Receives the decoder to set, and what output to enable
  //Disable targetDecoder so you can't see the input changing
  digitalWrite(EnableDecoder[targetDecoder], LOW);
  //Set decoder input
  if (targetDecoder == 3 && targetOutput == 0) digitalWrite(col25, HIGH);
  else {
    //If the A input of decoder (first binary digit) needs to be high (or 1)
    if (targetOutput == 1 || targetOutput == 3 || targetOutput == 5 || targetOutput == 7) digitalWrite(DecoderPinA[targetDecoder], HIGH);
    else digitalWrite(DecoderPinA[targetDecoder], LOW);
    //If the B input of decoder (second binary digit) needs to be high (or 1)
    if (targetOutput == 2 || targetOutput == 3 || targetOutput == 6 || targetOutput == 7) digitalWrite(DecoderPinB[targetDecoder], HIGH);
    else digitalWrite(DecoderPinB[targetDecoder], LOW);
    //If the C input of decoder (third binary digit) needs to be high (or 1)
    if (targetOutput >= 4) digitalWrite(DecoderPinC[targetDecoder], HIGH);
    else digitalWrite(DecoderPinC[targetDecoder], LOW);
    //Ensure that the target decoder is enabled
    digitalWrite(EnableDecoder[targetDecoder], HIGH);
  }
}

void SetLayer(int targetLayer, String desiredStatus, int brightness) {
  //Accepts a layer number and On or Off to make apporopriate changes
  if (desiredStatus == "On" || desiredStatus == "on" || desiredStatus == "1") {
    if (brightness==255) digitalWrite(Layer[targetLayer], HIGH);
    else analogWrite(Layer[targetLayer], brightness); 
  }
  if (desiredStatus == "Off" || desiredStatus == "off" || desiredStatus == "0")   digitalWrite(Layer[targetLayer], LOW);
}



void FullReset() {
  //Turn off all columns and layers
  //Used when lighting individual LED
  //Could add logic to check if an existing LED on column or layer is lit and leave it
  for (int x = 0; x<=2; x++) digitalWrite(EnableDecoder[x], LOW); //Turn off each decoder
  for (int x = 0; x<=5; x++) digitalWrite(Layer[x], LOW); //Turn off each layer
  digitalWrite(col25, LOW);
}

void AllLayersOn() {
  //Sets pins to high for all layers
  digitalWrite(Layer[0], HIGH);
  digitalWrite(Layer[1], HIGH);
  digitalWrite(Layer[2], HIGH);
  digitalWrite(Layer[3], HIGH);
  digitalWrite(Layer[4], HIGH);
}
 
void LayerWalk(int delayTime){
  //Walks all LED from lowest to highest
  for (int i=0; i < layersTotal * columnsTotal; i++) {
    led(i);
    delay(delayTime);
  }
}

int freeRam () {
  //http://playground.arduino.cc/Code/AvailableMemory
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

