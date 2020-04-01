#include <TM1638.h>
#define trigPin 13
#define echoPin 12
#define przesuniecie 4
#define proby 100

TM1638 module(8, 9, 10); //odpowiednie numery pinów arduino led key tm1638
byte displayDigits[] = {63,6,91,79,102,109,125,7,127,103 };
byte values[] = { 0,0,0,0,0,0,0,0 };
int cyfry[] = { 0,0,0,0,0,0,0,0 };
bool turn_on = false;
bool clicked = false;

void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  module.setDisplay(values);
  module.setupDisplay(true, 2);
}

void loop() 
{ //ustawienie przycisków S1 oraz S2
  byte keys = module.getButtons();
  if(keys == 1) 
    turn_on = true;
  if(keys == 2){
    turn_on = false;
    clicked = false;
  }
  if (turn_on && !clicked){
    double duration[proby], odleglosc;
    int milimeters;
    
    for (int i=0; i<proby; i++){
      digitalWrite(trigPin, LOW);        
      delayMicroseconds(2);              
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);           
      digitalWrite(trigPin, LOW);
      duration[i] = pulseIn(echoPin, HIGH);
      delay(10);

    }
    double srednia = 0;
    
    for (int i=0; i<proby;i++){
          srednia+=duration[i];  // liczenie średniej wartości
    }
   
    srednia /= proby;
    odleglosc = srednia/5.82;  //dzielenie prędkości przez stałą aby odległość w mm była prawidłowa
    milimeters = (int) odleglosc;
    
    delay(1000);
    
    displayNumber(milimeters);
    clicked = true;
  }
  else if (!clicked){
    display_off();
  }
}

void display_off(){
  
 for(int i = 0; i < 8; i++)
  values[i] = 64;
 
 module.setDisplay(values);
 delay(100);
 
 for(int i = 0; i < 8; i++)
  values[i] = 0;
  
 module.setDisplay(values);
   delay(100);
}

void displayNumber(int number_mm){
 // wyświrtlanie cyfr
 cyfry[0] = (int)(number_mm/1000);
 cyfry[1] = (int)((number_mm - (1000*cyfry[0])) / 100);
 cyfry[2] = (int)((number_mm - (1000*cyfry[0]) - (100*cyfry[1]))/10);
 cyfry[3] = (int)(number_mm - (1000*cyfry[0]) - (100*cyfry[1]) - (10*cyfry[2]));


 //przesunięcie cyfr na wyświetlaczu o 4 w prawo
 values[0+przesuniecie] = displayDigits[cyfry[0]];
 values[1+przesuniecie] = displayDigits[cyfry[1]]; 
 values[2+przesuniecie] = displayDigits[cyfry[2]] + 128;
 values[3+przesuniecie] = displayDigits[cyfry[3]];


 module.setDisplay(values); 
 
}
