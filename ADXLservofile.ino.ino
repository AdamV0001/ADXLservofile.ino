//declare variables for the motor pins
//Motor3 pins
int motorUppNedPin1 = 9;   // Blue - 28BYJ48 pin 9
int motorUppNedPin2 = 10;  // Pink - 28BYJ48 pin 10
int motorUppNedPin3 = 11;  // Yellow - 28BYJ48 pin 11
int motorUppNedPin4 = 12;  // Orange - 28BYJ48 pin 12
const int knapp1 = 7;
const int knapp2 = 6;

int motorSpeed = 1200;  //variable to set stepper speed
int lookup[8] = { B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001 };

void setup() {
  //declare the motor pins as outputs
  pinMode(motorUppNedPin1, OUTPUT);
  pinMode(motorUppNedPin2, OUTPUT);
  pinMode(motorUppNedPin3, OUTPUT);
  pinMode(motorUppNedPin4, OUTPUT);
  Serial.begin(9600);

  //declare the buttons
  pinMode(knapp1, INPUT);
  pinMode(knapp2, INPUT);
}

void loop() {
  // when button pressed, motor spins upward
  if (digitalRead(knapp1) == 1) {

    uppat();
  }
  // when other button pressed, motor spins downward
  if (digitalRead(knapp2) == 1) {

    nerat();
  }
}


void nerat() {
  // Loop to move the motor downward
  for (int i = 0; i < 8; i++) {
    setOutputUppNed(i); // Set motor output pins based on current step
    delayMicroseconds(motorSpeed); // Delay for motor speed
    Serial.print("Ned "); // Print "Ned" to Serial monitor
  }
}

void uppat() {
  // Loop to move the motor upward
  for (int i = 7; i >= 0; i--) {
    setOutputUppNed(i); // Set motor output pins based on current step
    delayMicroseconds(motorSpeed); // Delay for motor speed
    Serial.print("Upp "); // Print "Upp" to Serial monitor
  }
}

void setOutputUppNed(int out) {
  // Set motor output pins based on the value of 'out'
  digitalWrite(motorUppNedPin1, bitRead(lookup[out], 0));
  digitalWrite(motorUppNedPin2, bitRead(lookup[out], 1));
  digitalWrite(motorUppNedPin3, bitRead(lookup[out], 2));
  digitalWrite(motorUppNedPin4, bitRead(lookup[out], 3));
}
