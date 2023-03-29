#include <LiquidCrystal.h>
#include <DHT.h>
#include <Servo.h>
#include <Stepper.h>
#define Type DHT11

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 9, 8, 7, 6);

int pos = 0;    // variable to store the servo position
int dhtPin = A5;
int ledPin = A2;
int buzzerPin = 13;
int trigPin = A4;
int echoPin = A3;
int fan = A1;

float humidity;
float tempC;
float distance, duration;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT HT(dhtPin, Type);
Servo myservo;  // create servo object to control a servo

void current_temp(int nth_day){
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Day "); 
  lcd.print(nth_day); 
  lcd.print("; Temp:"); 
  lcd.print(tempC);
  lcd.setCursor(0,1);
  lcd.print("Humid:");
  lcd.print(humidity);
  digitalWrite(ledPin, LOW);
  if (tempC >27 && humidity > 60){
    digitalWrite(ledPin, HIGH);
    digitalWrite(fan, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Turn the fan on!"); 
    delay(1000);
  }
  else{
    digitalWrite(fan, LOW);
  }
  delay(50);
}

void person_detection(){

  digitalWrite(trigPin, LOW);
    delay(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    
    noTone(buzzerPin);     
    if (distance < 3){
      digitalWrite(buzzerPin, HIGH); 
      tone(buzzerPin,  500, 0);  
      digitalWrite(ledPin, HIGH);         
      lcd.clear();
      lcd.print("A person Nearby!");
      delay(5000);
      digitalWrite(buzzerPin, LOW);
    }
    else{      
       noTone(buzzerPin);
    }
    
}

void setup() {
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(buzzerPin, OUTPUT);  
  myservo.attach(10);
  myStepper.setSpeed(60);
  HT.begin();
  lcd.begin(16, 2);
  Serial.begin(9600); 
  
  Serial.println("Artificial Hatching");
  lcd.print("Artificial Hatching");
  delay(3000);
}

void loop() {
  digitalWrite(ledPin, LOW);
  
  for (int days = 14; days<=21; days++){
    
    digitalWrite(buzzerPin, LOW);    
    for (pos = 0; pos <= 180; pos += 5) { 
    
    myservo.write(pos); 
    current_temp(days);

    person_detection();                     
  }

  for (pos = 180; pos >= 0; pos -= 5) { 
    myservo.write(pos);
    current_temp(days); 

    person_detection();                      
  }
  
  }
  lcd.clear();
  lcd.blink();
  lcd.print("21 Days are over!");
  myStepper.step(stepsPerRevolution);
  delay(5000);
 
}


