// hardware pins on arduino
#define LED 3
#define SWITCH1 4
#define SWITCH2 7
#define KNOB1 A2
#define KNOB2 A3
#define KNOB3 A4
#define KNOB4 A5
#define KNOB5 A6
#define KNOB6 A1

int counter = 0; // counter to blink led

void setup() {
  // start serial communication (to print to console)
  Serial.begin(9600); 
  // setup pins as input or output
  pinMode(LED, OUTPUT); 
  pinMode(SWITCH1, INPUT);
  pinMode(SWITCH2, INPUT);
}

void loop() {
  Serial.print(digitalRead(SWITCH1));
  Serial.print(" ");
  Serial.print(digitalRead(SWITCH2));
  Serial.print(" ");
  Serial.print(analogRead(KNOB1));
  Serial.print(" ");
  Serial.print(analogRead(KNOB2));
  Serial.print(" ");
  Serial.print(analogRead(KNOB3));
  Serial.print(" ");
  Serial.print(analogRead(KNOB4));
  Serial.print(" ");
  Serial.print(analogRead(KNOB5));
  Serial.print(" ");
  Serial.println(analogRead(KNOB6));
  
  blinkLed();
  delay(10);
}

void blinkLed(){
  counter+=10;
  if(counter > 999) counter = 0; 

  if(counter < 550) digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW); 
}
