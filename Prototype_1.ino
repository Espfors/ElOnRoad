int sensor[48]; // Vector containing Sensor value, four values for each sensor
int average[12]; // Vector containing the averager of the four latest sensor value for each sensor
int posIndex; // Index of the sensor detecting the road
int maxValue; // Value of the sensor detecting the road
int pos; // used for iterating of sensor[48]vector

void setup() {
  Serial.begin(9600); // Initiates the serial communication 
  // Setup digitalpin 0 to 11 as output, will be used to give feedback where the system findes the road
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  // Setup analogpin A0 to A11 as input, will be used for reading the IR-sensors
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  pinMode(A8,INPUT);
  pinMode(A9,INPUT);
  pinMode(A10,INPUT);
  pinMode(A11,INPUT);
  posIndex = 0; // Initiates index
  pos = 36; // Initiates vector index
}

void loop() {
  // Reads the sensor three times
  sensor[0] = analogRead(A0);
  sensor[1] = analogRead(A1);
  sensor[2] = analogRead(A2);
  sensor[3] = analogRead(A3);
  sensor[4] = analogRead(A4);
  sensor[5] = analogRead(A5);
  sensor[6] = analogRead(A6);
  sensor[7] = analogRead(A7);
  sensor[8] = analogRead(A8);
  sensor[9] = analogRead(A9);
  sensor[10] = analogRead(A10);
  sensor[11] = analogRead(A11);
  sensor[12] = analogRead(A0);
  sensor[13] = analogRead(A1);
  sensor[14] = analogRead(A2);
  sensor[15] = analogRead(A3);
  sensor[16] = analogRead(A4);
  sensor[17] = analogRead(A5);
  sensor[18] = analogRead(A6);
  sensor[19] = analogRead(A7);
  sensor[20] = analogRead(A8);
  sensor[21] = analogRead(A9);
  sensor[22] = analogRead(A10);
  sensor[23] = analogRead(A11);
  sensor[24] = analogRead(A0);
  sensor[25] = analogRead(A1);
  sensor[26] = analogRead(A2);
  sensor[27] = analogRead(A3);
  sensor[28] = analogRead(A4);
  sensor[29] = analogRead(A5);
  sensor[30] = analogRead(A6);
  sensor[31] = analogRead(A7);
  sensor[32] = analogRead(A8);
  sensor[33] = analogRead(A9);
  sensor[34] = analogRead(A10);
  sensor[35] = analogRead(A11);
  while (true) { //Loop used for identification
    maxValue = 0; // resets max value
    sensor[pos] = analogRead(A0);
    sensor[pos+1] = analogRead(A1);
    sensor[pos+2] = analogRead(A2);
    sensor[pos+3] = analogRead(A3);
    sensor[pos+4] = analogRead(A4);
    sensor[pos+5] = analogRead(A5);
    sensor[pos+6] = analogRead(A6);
    sensor[pos+7] = analogRead(A7);
    sensor[pos+8] = analogRead(A8);
    sensor[pos+9] = analogRead(A9);
    sensor[pos+10] = analogRead(A10);
    sensor[pos+11] = analogRead(A11);
    digitalWrite(posIndex, LOW); // Turns the identifications led off
    for (int j = 0; j < 12; j++){
          average[j]= ((sensor[j]+sensor[j+12]+sensor[j+24]+sensor[j+36])>>2); // calculate average
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
    pos = (pos+12)%48; // increase pos by 12 module modulus 48
  }
}




