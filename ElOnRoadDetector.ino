int inputarray[10];
int in = A8;


void setup() {
  Serial.begin(9600);
  /*for (int i: inputarray) {
    pinMode(i, INPUT);
  }*/
  pinMode(A8,INPUT);
}

void loop() {
  /*for(int i: inputarray) {
    inputarray[i] = analogRead(i);
  }*/
  testOneSensor();

  delay(50);
}

void testOneSensor() {
  int val = analogRead(in);
  Serial.println(val);  
}

