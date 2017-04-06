int sensor[48]; // Vector containing Sensor value, four values for each sensor
int average[12]; // Vector containing the averager of the four latest sensor value for each sensor
int posIndex; // Index of the sensor detecting the road
int maxValue; // Value of the sensor detecting the road
int oldavg[12];
int pos; // used for iterating of sensor[48]vector
int motorPin = 17;
void setup() {
  Serial.begin(9600); // Initiates the serial communication for debugging

  // Setup digitalpin 0 to 11 as output, will be used to give feedback where the system findes the road
  for (int i = 0; i < 12; i++) {
    pinMode(i, OUTPUT);
  }
  // Setup analogpin A0 to A9 as input, will be used for reading the IR-sensors
  for (int i = 14; i < 24; i++) {
    pinMode(i, INPUT);
  }
  // Setup analogpin A12 and A13 as input, will be used for reading the IR-sensors
  pinMode(31, INPUT);
  pinMode(32, INPUT);
  posIndex = 0; // Initiates index
  pos = 0; // Initiates vector index
}

void loop() {
  // Reads the sensor three times
  while (pos < 36) {
    for (int i = 14; i < 24; i++) {
      sensor[pos] = analogRead(i);
      pos++;
    }
    sensor[pos] = analogRead(31);
    pos++;
    sensor[pos] = analogRead(32);
    pos++;
  }
  while (true) { //Loop used for identification
    maxValue = 0; // resets max value
    for (int i = 14; i < 24; i++){
      Sensor[pos] = analogRead(i);
      pos++;
    }
    sensor[pos] = analogRead(31);
    pos++;
    sensor[pos] = analogRead(32);
    pos++;
    digitalWrite(posIndex, LOW); // Turns the identifications led off
    for (int j = 0; j < 12; j++) {
      average[j] = ((sensor[j] + sensor[j + 12] + sensor[j + 24] + sensor[j + 36]) >> 2); // calculate average
      if (average[j] > oldavg[j] + 100 || average[j] < oldavg[j] - 100) { //Disregard if new average is much larger or smaller then old average, +-100 is a just a guess.
        average[j] = oldavg[j];
      } else {
        oldavg[j] = average[j];
      }
    }
    //Finds max value and index of max value
    for (int i = 0; i < 12; i++) {
      if (average[i] > maxValue) {
        maxValue = average[i];
        posIndex = i;
      }
    }
    Serial.println("Road at sensor: " + posIndex); // Print index to serial monitor
    digitalWrite(posIndex, HIGH); // Turns the identifications led on
    pos = (pos + 12) % 48; // increase pos by 12 module modulus 48
  }
  delay(50);
}
