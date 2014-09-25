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

//Function Prototypes to allow for default argument values
//Must be above the Setup line? http://forum.arduino.cc/index.php/topic,40799.0.html
void led(int targetLed, int brightness=255); 
void SetLayer(int targetLayer, String desiredStatus="on", int brightness=255);
void FadeLed(int targetLed, int runTime=1500, int minBrightness=0, int maxBrightness=255);
void FadeLayer(int targetLayer, int runTime=1500, int minBrightness=0, int maxBrightness=255);


void setup() {
  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  delay(1);
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);   
    digitalWrite(ledPins[thisPin], LOW);  
  }
  delay(1);
  Serial.begin(9600);
  Serial.println("Setup Done");
}





void loop() {
  //Main loop for various patterns
  int pattern = random(18);
  //Remark out the line below to run random patterns, or set the value to the pattern you want to display
  //pattern=19;
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
       LightFullCube(3000);
       break;
     case 13:
       DesignCheckerboard(5000);
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
       FullReset();
       delay(100);
       DesignExplode(250);
       delay(500);
       break;
     /*
     case 18:
       DesignSidewaysFill(1000);
       break;
     case 19:
       DesignDiagonalFill(1000);
       break;
    */
     case 20:
       DesignCheckerboardV2(3000);
       break;
      
    case 9000: 
      LayerWalk(25);
      WalkLayerUpDown(25);
      WalkLayerUpDownReverse(25);
      WalkSpiral(50);
      for (int x=0; x <= 30; x++) RandomLed(150); //30 random LEDs
      for (int x=0; x <= 2; x++) { 
        ColumnUpDown(random(25), 150); //Random up and down the column
        ColumnUp(random(25), 150); //Random up the column
        ColumnDown(random(100, 125), 150); //Random down the column
      }
      for (int x=0; x<15; x++) LayerWalk(1);
      CrawlFullCube(1000);
      FillFullCube(2000);
      for (int x=0; x<=15; x++) FadeLed(random(125));
      LightFullCube(5000);
      break;
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

void DesignDiagonalFill(int RunTime) {
  //Animation filling cube diagaonally
  ShowDiagonalFillPart1(RunTime/9*4);
  ShowDiagonalFillPart2(RunTime/9*4);
  ShowDiagonalFillPart3(RunTime/9*1);
}

 
void ShowDiagonalFillPart1(int RunTime) {
  int framecount=4;
  boolean frame1[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0}
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
      {0, 0, 0, 0, 0},
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
  ShowDesign(frame1, RunTime/framecount);
  boolean frame2[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
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
  ShowDesign(frame2, RunTime/framecount);
  boolean frame3[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0}
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
  ShowDesign(frame3, RunTime/framecount);
  boolean frame4[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    },
    {//Layer 3
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    },
    {//Layer 4
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}
    }
  };
  ShowDesign(frame4, RunTime/framecount);
}

void ShowDiagonalFillPart2(int RunTime) {
  int framecount=4;
  boolean frame5[5][5][5] = 
  {
    {//Layer 0
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame5, RunTime/framecount);
    boolean frame6[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame6, RunTime/framecount);
  boolean frame7[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame7, RunTime/framecount);
  boolean frame8[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame8, RunTime/framecount);
}
void ShowDiagonalFillPart3(int RunTime) {
  int framecount=1;
  boolean frame9[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame9, RunTime/framecount);
}

void DesignSidewaysFill(int RunTime) {
  DesignSidewaysFillPart1(RunTime/9*4);
  DesignSidewaysFillPart2(RunTime/9*4);
  DesignSidewaysFillPart3(RunTime/9*1);
}

void DesignSidewaysFillPart1(int RunTime) {
  //Animation filling cube sideways
  int framecount=4;
  boolean frame1[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0}
    },
    {//Layer 3
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0}
    },
    {//Layer 4
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0}
    }
  };
  ShowDesign(frame1, RunTime/framecount);
  boolean frame2[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0}
    },
    {//Layer 3
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0}
    },
    {//Layer 4
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0}
    }
  };
  ShowDesign(frame2, RunTime/framecount);
  boolean frame3[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0}
    },
    {//Layer 3
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0}
    },
    {//Layer 4
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0}
    }
  };
  ShowDesign(frame3, RunTime/framecount);
  boolean frame4[5][5][5] = 
  {
    {//Layer 0
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    },
    {//Layer 1
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    },
    {//Layer 2
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    },
    {//Layer 3
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    },
    {//Layer 4
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0}
    }
  };
  ShowDesign(frame4, RunTime/framecount);
}

void DesignSidewaysFillPart2(int RunTime) {
  //Animation filling cube sideways
  int framecount=4;
  boolean frame5[5][5][5] = 
  {
    {//Layer 0
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame5, RunTime/framecount);
  boolean frame6[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 1, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame6, RunTime/framecount);
  boolean frame7[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 1, 1, 0, 0},
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame7, RunTime/framecount);
  boolean frame8[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame8, RunTime/framecount);
}
void DesignSidewaysFillPart3(int RunTime) {
  //Animation filling cube sideways
  int framecount=1;
  boolean frame9[5][5][5] = 
  {
    {//Layer 0
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 1
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 2
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 3
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    },
    {//Layer 4
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1}
    }
  };
  ShowDesign(frame9, RunTime/framecount);
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
  int frame[5][5] = {
    {//Layer 0
      B10101, 
      B01010,
      B10101, 
      B01010,
      B10101
    },
    {//Layer 1
      B01010,
      B10101, 
      B01010,
      B10101,
      B10101 
    },
    {//Layer 2
      B10101, 
      B01010,
      B10101, 
      B01010,
      B10101
    },
    {//Layer 3
      B01010,
      B10101, 
      B01010,
      B10101,
      B10101 
    },
    {//Layer 4
      B10101, 
      B01010,
      B10101, 
      B01010,
      B10101
    }
  };
  ShowDesignV2(frame, RunTime);
}

void DesignCheckerboard(unsigned long RunTime) {
  //Every other LED in all directions
  boolean frame[5][5][5] = 
  {
    {//Layer 0
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1}
    },
    {//Layer 1
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0}
    },
    {//Layer 2
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1}
    },
    {//Layer 3
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0}
    },
    {//Layer 4
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1},
      {0, 1, 0, 1, 0},
      {1, 0, 1, 0, 1}
    }
  };
  ShowDesign(frame, RunTime);
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



void ShowDesignV2(int frame[5][5], unsigned long RunTime) {
  //Accepts a 5 array and displays the output simultaneous
  //Output is significantly dimmer than single as we are flashing off and on and limited power per layer
  
  /*
  boolean frameDecoderView [5][4][8];
     for (byte layer=0; layer<=4; layer++ {
        for (byte row=0; row<5; ++row) {
            //byte data = pgm_read_byte (&BitMap[x]);   // fetch data from program memory
            for (byte y=0; y<7; ++y) {
                if (frame[layer][row] & (1<<y)) {
                    // turn on the LED at location (x,y)
                } else {
                    // turn off the LED at location (x,y)
                }
            }
        }
     }
    */
    
unsigned long ledLine[5];
//Take all layer and row information and put into a long array for each layer to walk through
for (int layer=0; layer<=4; layer++) {
  for (int row=4; row>=0; row--) {
    if (row == 4) ledLine[layer]=frame[layer][4]; //For first entry, set the value to the lowest row, since the decoders are connected starting at the bottom if viewed from above.
    else ledLine[layer] = (ledLine[layer] << 5) | frame[layer][row]; //Shift the existing values to the left by 5 digits, then OR the next row into the empty area
  }
  ledLine[layer] = (ledLine[layer] << 7); //Shift further over by 7 digits to fill out the space for the 4th virtual decoder
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
            if (ledLine[layer] & (1<<31-(decoder*8)-output)) { //Need to shift over 31 digits to start at first position, if working on another decoder, move 8 less digits, and one less per output
          //if (ledLine[layer] & (2147483648 >>(decoder*8)+output)) //Possibly simpler to understand, as the starting point is at LED 0 and then shifting to the right
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

