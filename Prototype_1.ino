int sensor[12];
int posIndex;
int maxValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  pinMode(12, OUTPUT);
  posIndex = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  maxValue = 0;
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
  sensor[12] = analogRead(A12);
  digitalWrite(posIndex, LOW)
  for (int i = 0; i < 12; i++) {
    if (sensor[i] > maxValue) {
      maxValue = sensor[i];
      posIndex = i;
    }
  }
  Serial.println("Road at sensor: " + posIndex);
  digitalWrite(posIndex, High);
}

