/*
ElOnRoad Track Finder
Authors
Nils Espfors
Johan Lindqvist
Michael Gabassi
Emil Wåreus
Fredrik Olsson
*/

int sensor[65]; // Vector containing Sensor value, five values for each sensor
int average[13]; // Vector containing the averager of the five latest sensor value for each sensor
int posIndex[13]; // Vector containing which leds should indicate the road
int maxValue; // Current maximum value of the sensors
int pos; // used for iterating of sensor[65]vector
int ofset = 110; //Interval around max value
int startMotor = 800; //Upper accepteable limit for sensor values, values above this will indicate dirt on one or more senors
//and start the cleaning system. 
boolean clean = true; //System is clean
boolean noRoad = true; //No ElOnRoad track found
int limit = 320; //Lower accepteable limit for sensor values Needs to be recalibrated after the mounting of the detector

void setup() {
  //Serial.begin(9600); // Initiates the serial communication for debugging

  // Setup digitalpin 0 to 12 as output, will be used to give feedback to the user using the LED-display
  for (int i = 0; i < 13; i++) {
    pinMode(i, OUTPUT);
  }

  // Setup digitalpin 32 as output, will be used to controll the motor
  pinMode(32, OUTPUT);

  // Setup analogpin A0 (14) to A9 (23) as input, will be used for reading the IR-sensors
  for (int i = 14; i < 24; i++) {
    pinMode(i, INPUT);
  }

  // Setup analogpin A14 (33) to A16 (35) as input, will be used for reading the IR-sensors
  for (int i = 33; i < 36; i++) {
    pinMode(i, INPUT);
  }
  pos = 0; // Initiates vector index
}

void loop() {
  //Super awesome initiation
  digitalWrite(6, 1);
  delay(100);
  for (int i = 1; i < 7; i++) {
    digitalWrite(6 + i, 1);
    digitalWrite(6 - i, 1);
    delay(100);
  }
  for (int i = 6; i > 0; i--) {
    digitalWrite(6 + i, 0);
    digitalWrite(6 - i, 0);
    delay(100);
  }
  digitalWrite(6, 0);
  delay(100);
  digitalWrite(6, 1);
  delay(100);
  digitalWrite(6, 0);
  delay(100);
  digitalWrite(6, 1);
  delay(100);
  digitalWrite(6, 0);
  delay(100);

  // Reads the sensor four times
  while (pos < 40) {
    for (int i = 14; i < 24; i++) {
      sensor[pos] = analogRead(i);
      pos++;
    }
    for (int i = 33; i < 36; i++) {
      sensor[pos] = analogRead(i);
      pos++;
    }
  }

  while (true) { //Loop used for identification
    maxValue = limit; // Resets max value to lower limit
    //Reading sensor 0 - 10
    for (int i = 14; i < 24; i++) {
      sensor[pos] = analogRead(i);
      pos++;
    }
    //Reading sensor 11 - 13
    for (int i = 33; i < 36; i++) {
      sensor[pos] = analogRead(i);
      pos++;
    }

    //Moving average filter
    for (int j = 0; j < 13; j++) {
      int sum = 0;
      for (int k = 0; k < 5; k++) {
        sum = sum + sensor[j + k * 13];
      }
      average[j] = sum / 5;
    }
    
    //Finds max value and index of max value
    for (int i = 1; i < 12; i++) {
      //If the average value of sensor i is greater or equal to max value + ofset we check that
      //the sensor to the right or the sensor to the left is within range of the value of sensor i,
      //if they are we have found the road.
      if (average[i] >= (maxValue + ofset)) {
        memset(posIndex, 0, sizeof(posIndex)); // Filles posIndex with zeros
        if (average[i - 1] > (average[i] - ofset) && average[i - 1] < (average[i] + ofset)) { //Checks if average[i-1] is within intervall
          maxValue = average[i]; //Update maxValue
          posIndex[i] = 1; //LED at position i will be turned on
          posIndex[i - 1] = 1; //LED as position i-1 will be turned on
        }
        if (average[i + 1] > (average[i] - ofset) && average[i + 1] < (average[i] + ofset)) { //Checks if average[i+1] is within intervall
          maxValue = average[i]; //Update maxValue
          posIndex[i] = 1; //LED at position i will be turned on
          posIndex[i + 1] = 1; ////LED as position i+1 will be turned on
        }
        noRoad = false;
      }
    }
    //If no ElOnRoad track can be found, posIndex will be filles with zeros
    if (noRoad) {
      memset(posIndex, 0, sizeof(posIndex)); // Filles posIndex with zeros
    }
    noRoad = true;

    //Turns the motors on to start the cleaning system.
    //The cleaning system will keep running until the sensor values are low enought to indicate a clean system.
    if (maxValue > startMotor && clean) {
      digitalWrite(32, HIGH); //Turns motor on
      //Turns the red LEDs on to indicate that the cleaing system is running
      digitalWrite(0, 1);
      digitalWrite(1, 1);
      for (int i = 2; i < 11; i++) {
        digitalWrite(i, 0);
      }
      digitalWrite(11, 1);
      digitalWrite(12, 1);
      clean = false;
    }


    //Using led display to show Elonroad position
    else if (maxValue < startMotor) {
      digitalWrite(32, LOW); //Stopps the motor
      clean = true; 
      for (int i = 0; i < 13; i++) { //Iterates posIndex and turns LED on LED-display on or off corresponding to identification
        digitalWrite(i, posIndex[i]);
      }
    }
    pos = ((pos) % 65); // pos modulo 65
    delay(70); // 70ms delay
  }
}
