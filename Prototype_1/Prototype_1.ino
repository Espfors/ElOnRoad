int sensor[260]; // Vector containing Sensor value, four values for each sensor
int average[13]; // Vector containing the averager of the four latest sensor value for each sensor
int posIndex[13]; // Vector containing which leds should indicate the road
int maxValue; // Value of the sensor detecting the road
int pos; // used for iterating of sensor[52]vector
int ofset = 5; //Interval for max value
int startMotor = 850; //Upper accepteable limit for sensor values
boolean clean = true;
int limit = 330; //Lower accepteable limit for sensor values
void setup() {
  Serial.begin(9600); // Initiates the serial communication for debugging

  // Setup digitalpin 0 to 12 as output, will be used to give feedback where the system findes the road
  for (int i = 0; i < 13; i++) {
    pinMode(i, OUTPUT);
  }

  // Setup digitalpin 32 as output, will be used to start motors
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
    maxValue = limit; // Resets max value to lower limit
    memset(posIndex, 0, sizeof(posIndex)); // Reset led display
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
    for (int j = 0; j < 13; j++) {
      int sum = 0;
      for (int k = 0; k < 20; k++) {
        sum = sum + sensor[j + k * 13];
      }
      average[j] = sum / 20;
      //average[j] = ((sensor[j] + sensor[j + 13] + sensor[j + 26] + sensor[j + 39] + sensor[j+52] + sensor[j+65] + sensor[j+78] + sensor[j+91] + sensor[j+104] + sensor[j+117])/10); // calculate average
      Serial.print(average[j]);
      Serial.print(" ");
    }
    Serial.println();
    //Finds max value and index of max value
    for (int i = 0; i < 13; i++) {
      //If the average value is large than maxvalue+ofset,
      //then we reset the values of posIndex,
      //update the maxvalue and enters the led to posIndex
      if (average[i] >= (maxValue + 4*ofset)) {
        memset(posIndex, 0, sizeof(posIndex)); // Filles posIndex with zeros
        maxValue = average[i];
        posIndex[i] = 1;
        if (average[i - 1] > (maxValue - ofset) && average[i - 1] < (maxValue + ofset) && i > 1) {
          posIndex[i - 1] = 1;
        }
        if (average[i + 1] > (maxValue - ofset) && average[i + 1] < (maxValue + ofset) && i < 12) {
          posIndex[i + 1] = 1;
        }
      }
    }
    //If the average is in the interval of +- ofset the corresponding led is added to posIndex
    //    else if (average[i] > (maxValue - ofset) && average[i] < (maxValue + ofset)) {
    //      posIndex[i] = 1;
    //    }
    //}
    //Turns the motors on to start the cleaning system
    if (maxValue > startMotor && clean) {
      digitalWrite(32, HIGH);
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
      digitalWrite(32, LOW);
      clean = true;
      for (int i = 0; i < 13; i++) {
        digitalWrite(i, posIndex[i]);
      }
    }
    pos = ((pos) % 260); // pos modulo 130
    delay(50);
  }
}
