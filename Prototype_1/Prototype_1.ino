int sensor[52]; // Vector containing Sensor value, four values for each sensor
int average[13]; // Vector containing the averager of the four latest sensor value for each sensor
int posIndex[13]; // Vector containing which leds should indicate the road
int maxValue; // Value of the sensor detecting the road
int oldavg[13];
int pos; // used for iterating of sensor[52]vector
int ofset = 20; //Interval for max value
int startMotor = 700;
void setup() {
  Serial.begin(9600); // Initiates the serial communication for debugging

  // Setup digitalpin 0 to 12 as output, will be used to give feedback where the system findes the road
  for (int i = 0; i < 13; i++) {
    pinMode(i, OUTPUT);
  }
  // Setup digitalpin 24 as output, will be used to start motors
  pinMode(24, OUTPUT);

  // Setup analogpin A0 (14) to A9 (23) as input, will be used for reading the IR-sensors
  for (int i = 14; i < 24; i++) {
    pinMode(i, INPUT);
  }
  // Setup analogpin A14 (33)to A16 (35) as input, will be used for reading the IR-sensors
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
  digitalWrite(6,0);
  delay(100);
  digitalWrite(6,1);
  delay(100);
  digitalWrite(6,0);
  delay(100);
  digitalWrite(6,1);
  delay(100);
  digitalWrite(6,0);
  delay(100);
  // Reads the sensor three times
  while (pos < 30) {
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
    maxValue = 300+ofset; // resets max value
    memset(posIndex, 0, sizeof(posIndex));
    for (int i = 14; i < 24; i++) {
      sensor[pos] = analogRead(i);
      pos++;
    }
    for (int i = 33; i < 36; i++) {
      sensor[pos] = analogRead(i);
      pos++;
    }
    for (int j = 0; j < 13; j++) {
      average[j] = ((sensor[j] + sensor[j + 13] + sensor[j + 26] + sensor[j + 39]) >> 2); // calculate average
     // Serial.print(average[j]);
      //Serial.print(" ");
      //      if (average[j] > oldavg[j] + 100 || average[j] < oldavg[j] - 100) { //Disregard if new average is much larger or smaller then old average, +-100 is a just a guess.
      //        average[j] = oldavg[j];
      //      } else {
      //        oldavg[j] = average[j];
      //      }
      //      Serial.print("Average : ");
      //      Serial.println(average[j]);
    }
    //Serial.println();
    //Finds max value and index of max value
    for (int i = 0; i < 13; i++) {
      //If the average value is large than maxvalue+ofset,
      //then we reset the values of posIndex,
      //update the maxvalue and enters the led to posIndex
      if (average[i] > (maxValue + ofset)) {
        memset(posIndex, 0, sizeof(posIndex)); // Filles posIndex with zeros
        maxValue = average[i];
        posIndex[i] = 1;
      }
      //If the average is in the interval of +- ofset the corresponding led is added to posIndex
      else if (average[i] > (maxValue - ofset) && average[i] < (maxValue + ofset)) {
        posIndex[i] = 1;
      }
    }
    //Turns the motors on to start the cleaning
    if (maxValue > startMotor) {
      digitalWrite(24, HIGH);
      Serial.print(maxValue);
      Serial.println(" Motor is running");
    }
    //Turns leds on if posIndex[i] = 1
    else {
      for (int i = 0; i < 13; i++) {
        digitalWrite(i, posIndex[i]);

        if (posIndex[i] == 1) {
          Serial.print(average[i]);
          Serial.print(" ");
        }
      }
      Serial.println();
    }
    pos = ((pos) % 52); // pos modulo 48
    delay(50);
  }

}
