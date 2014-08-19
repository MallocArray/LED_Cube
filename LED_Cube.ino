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

void setup() {
  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);   
    digitalWrite(ledPins[thisPin], LOW);  
  }
//  Serial.begin(9600);
}





void loop() {
  //Main loop for various patterns
/*
  LayerWalk(25);
  WalkLayerUpDown(25);
  WalkLayerUpDownReverse(25);
  WalkSpiral(25);
  for (int x=0; x <= 30; x++) RandomLed(150); //30 random LEDs
  for (int x=0; x <= 2; x++) { 
    ColumnUpDown(random(25), 150); //Random up and down the column
    ColumnUp(random(25), 150); //Random up the column
    ColumnDown(random(100, 125), 150); //Random down the column
  }
  CrawlFullCube(1000);
  FillFullCube(2000);
  //DesignCube(2000);  Buggy.  Depends on what things were at before starting function
  LightFullCube(5000);
*/
//  WalkSpiral(100); //Still needs work.  Break out spiral in and out
  FadeLed(0, 0, 255, 1000);
  delay(3000)


//  LayerByLayerUp(250); //Will need POV to allow all decoder pins to be on
//  LayerByLayerDown(250); //Will need POV to allow all decoder pins to be on
//  ColumnByColumnToFull(250);
//  ColumnByColumnToEmpty(250);
}

void FadeLed(int targetLed, int minBrightness, int maxBrightness, int runTime) {
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
  //SetLayer(targetLayer, "On");
  FadeLayer(targetLayer, minBrightness, maxBrightness, runTime);
}



void FadeLayer(int targetLayer, int minBrightness, int maxBrightness, int runTime) {
  //Accepts a Layer, and brightness values, and runs for that long, fading the value in and out
  long StartTime = millis();
  unsigned long CurrentTime = millis();
  //Run through this function for the specified amount of time
  while (CurrentTime - StartTime <= runTime) {
    for(int fadeValue = minBrightness ; fadeValue <= maxBrightness; fadeValue +=5) { 
    analogWrite(Layer[targetLayer], fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
    } 
    // fade out from max to min in increments of 5 points:
    for(int fadeValue = maxBrightness ; fadeValue >= minBrightness; fadeValue -=5) { 
      // sets the value (range from 0 to 255):
      analogWrite(Layer[targetLayer], fadeValue);         
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);                            
    } 
    CurrentTime = millis();
  }
}


void DesignCube(int RunTime) {
  long StartTime = millis();
  unsigned long CurrentTime = millis();
  int DelayTime =1;
  int Frames = 5;
  led(0);
//  FullReset();
  while (CurrentTime - StartTime <= RunTime) {
    //Frame 1
    //FullReset();
//    for (int x=0; x<=2; x++) digitalWrite(EnableDecoder[x], LOW);  
    digitalWrite(EnableDecoder[0], LOW);
    digitalWrite(EnableDecoder[1], LOW);
    digitalWrite(EnableDecoder[2], LOW);
    for (int x=0; x<=4; x++) SetLayer(x, "On");
    //SetLayer(0,"On");
    //SetLayer(1,"On");
    //SetLayer(2,"On");
    //SetLayer(3,"On");
    //SetLayer(4,"On");
    digitalWrite(EnableDecoder[1], LOW);    
    SetDecoder(0,0);
    delay(DelayTime);
    SetDecoder(0,4);
    SetDecoder(2,4);
    digitalWrite(col25, HIGH);
    delay(DelayTime);  
    //Frame2;
    SetLayer(1,"Off");
    SetLayer(2,"Off");
    SetLayer(3,"Off");
    SetDecoder(0,1);
    SetDecoder(1,1);
    SetDecoder(2,3);
    digitalWrite(col25, LOW);
    delay(DelayTime);
    //Frame 3
    SetDecoder(0,2);
    SetDecoder(1,2);
    SetDecoder(2,4);
    digitalWrite(col25, HIGH);
    delay(DelayTime);
    //Frame 4
    SetDecoder(0,3);
    SetDecoder(1,6);
    SetDecoder(2,5);
    digitalWrite(col25, LOW);
    delay(DelayTime);
    //Frame 5
    SetDecoder(0,5);
    SetDecoder(1,7);
    SetDecoder(2,6);
    delay(DelayTime);
    //Frame 6
    SetDecoder(2,7);
    delay(DelayTime);
    CurrentTime = millis();
  }
  //FullReset();
}

void CrawlFullCube(int RunTime) {
  //Lights all LEDs on a single layer, crawling up and down the full cube
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
  long StartTime = millis();
  unsigned long CurrentTime = millis();
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
      digitalWrite(col25, HIGH); //To keep the last column in sync, it cycles off an on like the others
      SetDecoder(0, x);
      SetDecoder(1, x);
      SetDecoder(2, x);
      digitalWrite(col25, LOW);
      delayMicroseconds(250); //When using 1 millisecond, the 25th column looks dimmer
    }
    CurrentTime = millis();
  }
}

void LightFullCube (int RunTime) {
  //Lights all LED on the entire cube for a duration of milliseconds
  long StartTime = millis();
  unsigned long CurrentTime = millis();
  for (int x=0; x<=4; x++) SetLayer(x, "On"); //Turn all layers on
  //Run through this function for the specified amount of time
  CyclePins(RunTime);
  
  /*
  while (CurrentTime - StartTime <= RunTime) {
    //Cycle through each Decoder output, at a specified delay
    for (int x=0; x<=7; x++) {
      digitalWrite(col25, HIGH); //To keep the last column in sync, it cycles off an on like the others
      SetDecoder(0, x);
      SetDecoder(1, x);
      SetDecoder(2, x);
      digitalWrite(col25, LOW);
      delayMicroseconds(250); //When using 1 millisecond, the 25th column looks dimmer
    }
    CurrentTime = millis();
  }
  */
}


void RandomLed (int delayTime) {
  led(random(125));
  delay(delayTime);
}

void ColumnUpDown (int targetLed, int delayTime) {
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
 
void led(int targetLed) {
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
  SetLayer(targetLayer, "On");
}
 
void SetDecoder (int targetDecoder, int targetOutput) {
  //Receives the decoder to set, and what output to enable
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

void SetLayer(int targetLayer, String desiredStatus) {
  //Accepts a layer number and On or Off to make apporopriate changes
  if (desiredStatus == "On" || desiredStatus == "on" || desiredStatus == "1")   digitalWrite(Layer[targetLayer], HIGH);
  if (desiredStatus == "Off" || desiredStatus == "off" || desiredStatus == "0")   digitalWrite(Layer[targetLayer], LOW);
}



void FullReset() {
  //Turn off all columns and layers
  //Used when lighting individual LED
  //Could add logic to check if an existing LED on column or layer is lit and leave it
  for (int x = 0; x<=2; x++) digitalWrite(EnableDecoder[x], LOW);
  for (int x = 0; x<=5; x++) digitalWrite(Layer[x], LOW);
  digitalWrite(col25, LOW);
}

void AllColumnsHigh() {
  //Sets pins to HIGH for all columns
  //Broken as this only enables Pin 8 of each decoder
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], HIGH);
  digitalWrite(DecoderPinA[1], HIGH);
  digitalWrite(DecoderPinB[1], HIGH);
  digitalWrite(DecoderPinC[1], HIGH);
  digitalWrite(DecoderPinA[2], HIGH);
  digitalWrite(DecoderPinB[2], HIGH);
  digitalWrite(DecoderPinC[2], HIGH);
  digitalWrite(col25, HIGH);
}

void AllLayersHigh() {
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

    
void LayerByLayerUp(int DelayTime) {
  //Starts with all layers off and all columns HIGH
  //Adds one column at a time until all are on
  //Can consolidate with a for loop
  FullReset();
  AllColumnsHigh();
  digitalWrite(Layer[0], HIGH);
  delay(DelayTime);
  digitalWrite(Layer[1], HIGH);
  delay(DelayTime);
  digitalWrite(Layer[2], HIGH);
  delay(DelayTime);
  digitalWrite(Layer[3], HIGH);
  delay(DelayTime);
  digitalWrite(Layer[4], HIGH);
}

void LayerByLayerDown(int DelayTime) {
  //Can cosolidate with For loop
  AllColumnsHigh();
  AllLayersHigh();
  digitalWrite(Layer[4], LOW);
  delay(DelayTime);
  digitalWrite(Layer[3], LOW);
  delay(DelayTime);
  digitalWrite(Layer[2], LOW);
  delay(DelayTime);
  digitalWrite(Layer[1], LOW);
  delay(DelayTime);
  digitalWrite(Layer[0], LOW);
}

void ColumnByColumnToFull(int DelayTime) {
  //Starts with all columns and layers Low, and adds one column at a time
  //Continues until all are lit
  FullReset();
  AllLayersHigh();
  digitalWrite(DecoderPinA[0], LOW);
  digitalWrite(DecoderPinB[0], LOW);
  digitalWrite(DecoderPinC[0], LOW);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], LOW);
  digitalWrite(DecoderPinC[0], LOW);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], LOW);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], LOW);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], LOW);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], LOW);
  digitalWrite(DecoderPinB[0], LOW);
  digitalWrite(DecoderPinC[0], HIGH);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], LOW);
  digitalWrite(DecoderPinC[0], HIGH);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], LOW);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], HIGH);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], HIGH);
  delay(DelayTime);
}

void ColumnByColumnToEmpty(int DelayTime) {
  //Starts with all columns and layers, and removes one column at a time
  //Continues until empty
  AllColumnsHigh();
  AllLayersHigh();
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], HIGH);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], LOW);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], HIGH);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], LOW);
  digitalWrite(DecoderPinC[0], HIGH);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], LOW);
  digitalWrite(DecoderPinB[0], LOW);
  digitalWrite(DecoderPinC[0], HIGH);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], LOW);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], LOW);
  digitalWrite(DecoderPinB[0], HIGH);
  digitalWrite(DecoderPinC[0], LOW);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], HIGH);
  digitalWrite(DecoderPinB[0], LOW);
  digitalWrite(DecoderPinC[0], LOW);
  delay(DelayTime);
  digitalWrite(DecoderPinA[0], LOW);
  digitalWrite(DecoderPinB[0], LOW);
  digitalWrite(DecoderPinC[0], LOW);
  delay(DelayTime);
}
