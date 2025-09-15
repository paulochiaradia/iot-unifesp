const int entrada1 = 7;
const int entrada2 = 6;

const int led1Pin = 13;
const int led2Pin = 10;

// Intervalos para piscar os LEDs
const int led1BlinkInterval = 100;
const int led2BlinkInterval = 50;

// Variáveis para controlar o pisca-pisca (blink) sem usar delay()
unsigned long led1BlinkMillis = 0;
unsigned long led2BlinkMillis = 0;
bool led1State = LOW;
bool led2State = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(entrada1, INPUT);
  pinMode(entrada2, INPUT);
}

void loop() {
  int estado1 = digitalRead(entrada1);
  int estado2 = digitalRead(entrada2);
  
  if (estado1 == 0 && estado2 == 0) {
    idle();
  } else if (estado1 == 1 && estado2 == 0) {
    cleaning();
  } else if (estado1 == 0 && estado2 == 1) {
    docking();
  } else { // estado1 == 1 && estado2 == 1
    charging();
  }
}

void idle() {
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  Serial.println("IDLE"); 
}

void cleaning() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - led1BlinkMillis >= led1BlinkInterval) {
    led1BlinkMillis = currentMillis;
    led1State = !led1State;
    digitalWrite(led1Pin, led1State);
  }
  
  digitalWrite(led2Pin, LOW);
  Serial.println("CLEANING");
}

void docking() {
  unsigned long currentMillis = millis();

  digitalWrite(led1Pin, LOW);

  if (currentMillis - led2BlinkMillis >= led2BlinkInterval) {
    led2BlinkMillis = currentMillis;
    led2State = !led2State;
    digitalWrite(led2Pin, led2State);
  }
  Serial.println("DOCKING");
}

void charging() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - led1BlinkMillis >= led1BlinkInterval) {
    led1BlinkMillis = currentMillis;
    led1State = !led1State;
    digitalWrite(led1Pin, led1State);
  }
  
  if (currentMillis - led2BlinkMillis >= led2BlinkInterval) {
    led2BlinkMillis = currentMillis;
    led2State = !led2State;
    digitalWrite(led2Pin, led2State);
  }
  Serial.println("CHARGING");
}