#define Motor1Pin1 3
#define Motor1Pin2 5
// #define Motor1Pin1 6
// #define Motor1Pin1 9


const int dcMotors[] = {Motor1Pin1, Motor1Pin2};
const int forward[] = {HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW};
const int backward[] = {LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH};
const int STOP[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
const int motorSpeed[] = {100, 200};

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < sizeof(dcMotors)/sizeof(dcMotors)[0]; i++)
  {
    pinMode(dcMotors[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < sizeof(dcMotors) / sizeof(dcMotors)[0]; i++)
  {
    analogWrite(i, motorSpeed[0]);
  }
  delay(1000);

  for (int i = 0; i < sizeof(dcMotors) / sizeof(dcMotors)[0]; i++)
  {
    digitalWrite(dcMotors[i], STOP[i]);
  }
  delay(1000);
   
  for (int i = 0; i < sizeof(dcMotors) / sizeof(dcMotors)[0]; i++)
  {
    analogWrite(i, motorSpeed[1]);
  }
  delay(1000);

    for (int i = 0; i < sizeof(dcMotors) / sizeof(dcMotors)[0]; i++)
  {
    digitalWrite(dcMotors[i], STOP[i]);
  }
    delay(1000);
} 

