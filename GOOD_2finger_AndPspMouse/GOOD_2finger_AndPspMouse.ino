
const int xAxis = A5;         //analog sensor for X axis  
const int yAxis = A4;         // analog sensor for Y axis

int FingerV2 = 1; //powerglove index finger , on analog 3
int FingerV4 = 2; //powerglove ring finger , on analog 5



//variables for pushbutton
int fingerstate;             // the current reading from the input pin
int fingbool=1;
int fingerstate4;             // the current reading from the input pin
int fingbool4=1;

int lastfingerstate = 1;   // the previous reading from the input pin
int lastfingerstate4 = 1;   // the previous reading from the input pin
// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long lastDebounceTime4 = 0;  // the last time the output pin was toggled
long debounceDelay = 100;    // the debounce time; increase if the output flickers

int avg=29; //average value b/w open and closed finger positions
  
int avg4=51; //average value b/w open and closed finger positions
 //mouse stuff is below:


int range = 12;               // sensitivity: output range of X or Y movement
//int range =6; 
int responseDelay = 2;       // response delay of the mouse, in ms
//int threshold = range/4;      // resting threshold
int threshold = range/6;
int center = range/2;   // resting position value
//int center = range/3;
int minima[] = { 
  1023, 1023};                // actual analogRead minima for {x, y}
int maxima[] = {
  0,0};                       // actual analogRead maxima for {x, y}
int axis[] = {
  xAxis, yAxis};              // pin numbers for {x, y}
int mouseReading[2];          // final mouse readings for {x, y}

void setup() {
 // initialize the button pin as a input:
  Serial.begin(9600);
   Mouse.begin();

}

void loop() {
  fingbool= (analogRead(FingerV2))/avg;
  fingbool4= (analogRead(FingerV4))/avg4;
//larger than avg= greater than 1, true
   //smaller than avg= less than 1, false
                         
  // check to see if you just pressed the button 
  // (i.e. the input went from 0 to 1),  and you've waited 
  // long enough since the last press to ignore any noise: 
  // If the switch changed, due to noise or pressing:
  if (fingbool != lastfingerstate) {
    lastDebounceTime = millis();  // reset the debouncing timer
  } 
   if (fingbool4 != lastfingerstate4) {
    lastDebounceTime4 = millis();  // reset the debouncing timer
  } 
  //pointer finger = single click
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

   // if the button state has changed:
    if (fingbool != fingerstate) {
      fingerstate = fingbool;
   
    if (fingbool == 0) {
        Serial.println("Closed2   ");
         Mouse.press(MOUSE_LEFT);
      }
      else  {
        Serial.println("Open2   ");
       Mouse.release(MOUSE_LEFT);
      }
    }
  }  
  //ring finger = doubleclick
    if ((millis() - lastDebounceTime4) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

   // if the button state has changed:
    if (fingbool4 != fingerstate4) {
      fingerstate4 = fingbool4;
   
    if (fingbool4 == 0) {
          Serial.println("Closed4   ");
         Mouse.press(MOUSE_RIGHT);
      }
      else { 
        Serial.println("Open4   ");
        Mouse.release(MOUSE_RIGHT);
    }
  } 
    }
    
  lastfingerstate = fingbool;
lastfingerstate4 = fingbool4;
//mouse stuff is below
// read and scale the two axes:
  int xReading = readAxis(0);
  int yReading = readAxis(1);
  
  if (xReading > 0){
  xReading++;}
  if (yReading < 0){
 yReading--;}

// move the mouse:
    Mouse.move(xReading, yReading, 0);
    delay(responseDelay);
    
  /*  if ((millis() - lastDebounceTime) > debounceDelay) {
      Serial.print("X: ");
    Serial.print(xReading);
    Serial.print("   Y: ");
    Serial.println(yReading);
        lastDebounceTime = millis(); 
    }*/
}

/*
  reads an axis (0 or 1 for x or y) and scales the 
  analog input range to a range from 0 to <range>
*/

int readAxis(int axisNumber) {
  int distance = 0;    // distance from center of the output range

  // read the analog input:
  int reading = analogRead(axis[axisNumber]);

// of the current reading exceeds the max or min for this axis,
// reset the max or min:
  if (reading < minima[axisNumber]) {
    minima[axisNumber] = reading;
  }
  if (reading > maxima[axisNumber]) {
    maxima[axisNumber] = reading;
  }

  // map the reading from the analog input range to the output range:
  reading = map(reading, minima[axisNumber], maxima[axisNumber], 0, range);

 // if the output reading is outside from the
 // rest position threshold,  use it:
  if (abs(reading - center) > threshold) {
    distance = (reading - center);
  } 

  // the Y axis needs to be inverted in order to 
  // map the movemment correctly:
  if (axisNumber == 1) {
    distance = -distance;
  }

  // return the distance for this axis:
  return distance;
}

