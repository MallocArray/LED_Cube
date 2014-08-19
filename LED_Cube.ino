int ledPins[] = {2, 3, 4, 8, 12, 13, A1, A2, A3, A5, 5, 6, 9, 10, 11, 7, A0, A4 }; // an array of pin numbers to which LEDs are attached
int pinCount = 18; 

/*Each decoder has an A, B, and C pin
Setup an array for each pin on each decoder.
First decoder is [0] followed by [1], etc.
*/
const int DecoderPinA[3] = {2,8,A1};
const int DecoderPinB[3] = {3,12,A2};
const int DecoderPinC[3] = {4,13,A3};

//Last column is not connected to line decoder, but directly to an Arduino pin and transistor
const int col25 = A5;

//Each layer is directly connected to Arduino
const int Layer[5] = {5,6,9,10,11};
const int EnableDecoder[3] = {7,A0,A4};

const int layersTotal = 5; //Total number of layers in cube
const int columnsTotal = 25; //Total number of columns in a layer 5x5

void setup() {
  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);   
    digitalWrite(ledPins[thisPin], LOW);  
  }
  Serial.begin(9600);
}

void loop() {
  //Main loop for various patterns
//  LayerWalk(50);
//  WalkLayerUpDown(80);
//  WalkLayerUpDownReverse(80);
  WalkSpiral(150);
  delay(5000);


//  LayerByLayerUp(250); //Will need POV to allow all decoder pins to be on
//  LayerByLayerDown(250); //Will need POV to allow all decoder pins to be on
//  ColumnByColumnToFull(250);
//  ColumnByColumnToEmpty(250);
}

int WalkSpiral(int delayTime) {
  //Starting at 0, spiral around the perimeter spiraling in to the middle
  int interval = 5;
  int iteration = 4;
  int i = 0;
  int direction = 1;
  
  for (;i != 125;) {
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
  //Divide the targetLED by the total number of columns in a layer
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
  //Ensure everything is off before turning on a single LED
  FullReset();
  //If the A input of decoder (first binary digit) needs to be high (or 1)
  if (targetDecoderInput == 1 || targetDecoderInput == 3 || targetDecoderInput == 5 || targetDecoderInput == 7) {
    digitalWrite(DecoderPinA[targetDecoder], HIGH);
  }
  else {
    digitalWrite(DecoderPinA[targetDecoder], LOW);
  }
  //If the B input of decoder (second binary digit) needs to be high (or 1)
  if (targetDecoderInput == 2 || targetDecoderInput == 3 || targetDecoderInput == 6 || targetDecoderInput == 7) {
    digitalWrite(DecoderPinB[targetDecoder], HIGH);
  }
  else {
    digitalWrite(DecoderPinB[targetDecoder], LOW);
  }
  //If the C input of decoder (third binary digit) needs to be high (or 1)
  if (targetDecoderInput >= 4) {
    digitalWrite(DecoderPinC[targetDecoder], HIGH);
  }
  else {
    digitalWrite(DecoderPinC[targetDecoder], LOW);
  }
  //Special handling for column 25 as it is directly connected to Arduino and not a line decoder
  if (targetColumn == 24) {
    digitalWrite(col25, HIGH);
  }
  //Ensuare that the target decoder is enabled
  digitalWrite(EnableDecoder[targetDecoder], HIGH);
  //Enable power to the target layer
  digitalWrite(Layer[targetLayer], HIGH);
}
 

void FullReset() {
  //Turn off all columns and layers
  //Used when lightning individual LED
  //Potential bug/enhancement is that cannot have 2 concurrent LEDs
  //Could add logic to check if an existing LED on column or layer is lit and leave it
  for (int x = 0; x<=2; x++) {
    digitalWrite(DecoderPinA[x], LOW);
    digitalWrite(DecoderPinB[x], LOW);
    digitalWrite(DecoderPinC[x], LOW);
    digitalWrite(EnableDecoder[x], LOW);
  }
  for (int x = 0; x<=5; x++) {
    digitalWrite(Layer[x], LOW);
  }
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
