  #include <Servo.h> // folosim libraria servo a pentru controla servomotorul
  Servo servo1; // cream un obiect de tip servo

  // ~
  int trigPin1 = 9;
  int echoPin1 = 10;
  int servoPin1 = 3;
  int servoJos=0;
  int servoSus=70;
  //~ definim pinii pe care ii vom folosi pt,cat si limitele pt bariera
void setup() {
  

  pinMode(trigPin1, OUTPUT);  
	pinMode(echoPin1, INPUT);
  servo1.attach(3); // atasam servo la un pin 
	Serial.begin(9600); 
  servo1.write(servoJos); // initial bariera este coborata 
  
}

void loop() {
  //~
  float duration, distance;  
  digitalWrite(trigPin1, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin1, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin1, LOW);  
  duration = pulseIn(echoPin1, HIGH);  
  distance = (duration*.0343)/2;
  // ~ aici calculam distanta de la senzorul ultrasonic pana la obiectul din fata lui

  // ~
  if(distance<10)
  {
    servo1.write(servoSus);
    delay(5000);
  }
  // ~ daca obiectul este suficient de aproape de senzor, bariera se va deschside si va ramane deschisa timp de 5 secunde

  if(servo1.read()==servoSus)
    servo1.write(servoJos); // daca bariera este ridicata, o coboram
	delay(100);  
  
}
