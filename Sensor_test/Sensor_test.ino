int sensor[4];
int value;
int pin = 14;
int pos;
void setup() {
  pinMode(pin,INPUT);
  Serial.begin(9600);
  for (int i = 0; i< 3;i++){
    sensor[i] = analogRead(pin);
    delay(500);
  }
  pos = 3;
}

void loop() {
  sensor[pos] = analogRead(pin);
  pos = ((pos+1)%4);
  value = ((sensor[0]+sensor[1]+sensor[2]+sensor[3])/4);
  Serial.print("Average value: ");
  Serial.println(value);
  delay(1000);
}
