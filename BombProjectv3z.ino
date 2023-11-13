
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Tone.h>
#define pound 14
             
Tone tone1;

int Scount = 12; // count seconds
int Mcount = 1; // count minutes
int Hcount = 0; // count hours
int DefuseTimer = 0; // set timer to 0

long secMillis = 0; // store last time for second add
long interval = 1000; // interval for seconds

char password[4]; // number of characters in our password
int currentLength = 0; //defines which number we are currently writing
int i = 0; 
char entered[4];

int ledPin = 43; //red led
int ledPin2 = 45; //yellow led
int ledPin3 = 47; //green led

LiquidCrystal lcd(41,39,37,35,33,31); // the pins we use on the LCD

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A7, A6, A5, A4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A3, A2, A1}; //connect to the column pinouts of the keypad

//joystick

int Xaxisjoy;
int Yaxisjoy;
int Axisjoy;
int input[5];
int output[5];

int stage;
int winning;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup(){
  pinMode(A9, INPUT);
  pinMode(A8, INPUT);
  pinMode(ledPin, OUTPUT); // sets the digital pin as output
  pinMode(ledPin2, OUTPUT); // sets the digital pin as output
  pinMode(ledPin3, OUTPUT); // sets the digital pin as output
  tone1.begin(53);
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Keypad Code: ");
  


  
  while (currentLength < 4)
  {
    lcd.setCursor(currentLength + 6, 1);
    lcd.cursor();
    char key = keypad.getKey();
    key == NO_KEY;
    if (key != NO_KEY)
    {
      if ((key != '*')&&(key != '#'))
      { 
      lcd.print(key);
      password[currentLength] = key;
      currentLength++;
      tone1.play(NOTE_C6, 200);
      }
    }
  }

  if (currentLength == 4)
  {
    delay(500);
    lcd.noCursor();
    lcd.clear();
    lcd.home();
    lcd.print("Layer 1 Armed: ");
    lcd.setCursor(6,1);
    lcd.print(password[0]);
    lcd.print(password[1]);
    lcd.print(password[2]);
    lcd.print(password[3]);

    tone1.play(NOTE_E6, 200);
    delay(2000);
    lcd.clear();
    lcd.print("Hit * to Diffuse: ");
    delay(2000);
    lcd.clear();
    currentLength = 0;
  }
  lcd.print("Enter Joystick Code:");

}

void loop()
{
    if (Xaxisjoy == 2 ) {
    Axisjoy = Yaxisjoy;
  } else {
    Axisjoy = Xaxisjoy;
  }
     if (stage == 0) {
      Xaxisjoy = map(analogRead(A8), 0, 1000, 1, 3);
      Yaxisjoy = map(analogRead(A9), 0, 1000, 4, 6);
      
    if (Axisjoy != 5) {
      delay(500);


      for (int i = 0; i < 5; i++) {
        if (input[i] == 0) {
          input[i] = Axisjoy;
                    Serial.print("input "); Serial.print(i); Serial.print(" is ");
          Serial.println(input[i]);

          lcd.setCursor(6,1);
          
          //lcd.print("input "); lcd.print(i); lcd.print(" is ");
          lcd.print(input[0]);
          lcd.print(input[1]);
          lcd.print(input[2]);
          lcd.print(input[3]);
          lcd.print(input[4]);
        

        }
      }

    }
    if (input[4] != 0) {
      stage = 2;
       lcd.clear();
      Serial.println("stage 2 ");
      Axisjoy=5;
    }
  }


  Serial.println(stage);
 /* if (stage == 0) {
    if (Axisjoy != 5) {
      delay(500);


      for (int i = 0; i < 5; i++) {
        if (input[i] == 0) {
          input[i] = Axisjoy;

         // lcd.print("input "); lcd.print(i); lcd.print(" is ");
          //lcd.print(input[i]);
          Serial.print("input "); Serial.print(i); Serial.print(" is ");
          Serial.println(input[i]);
          break;
        }
      }

    }
    if (input[4] != 0) {
      stage = 2;
    }
  }*/
   
  if (stage ==2 ) { timer();}
  
  char key2 = keypad.getKey(); // get the key
  
  if (key2 == '*')
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Code: ");
      
      while (currentLength < 4)
        {
          
          timer();
          
          char key2 = keypad.getKey(); 
          if (key2 == '#')
            {
              currentLength = 0;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Code: ");
            }
          else                  
          if (key2 != NO_KEY)    
            {
              
              lcd.setCursor(currentLength + 7, 0);
              lcd.cursor();
              
              lcd.print(key2);
              entered[currentLength] = key2;
              currentLength++;
              tone1.play(NOTE_C6, 200);
              delay(100);
              lcd.noCursor();
              lcd.setCursor(currentLength + 6, 0);
              lcd.print("*");
              lcd.setCursor(currentLength + 7, 0);
              lcd.cursor();
            }
        }

      if (currentLength == 4) 
        {
          if (entered[0] == password[0] && entered[1] == password[1] && entered[2] == password[2] && entered[3] == password[3])
            {
              lcd.noCursor();
              lcd.clear();
              lcd.home();
              //lcd.print("Bomb Defused");
              currentLength = 0;
              digitalWrite(ledPin3, HIGH);
              delay(2500);
              //lcd.setCursor(0,1);
              //lcd.print("Reset the Bomb");
              //delay(1000000);
              stage = 3; // move to stage 3( solving joystick)
            }
      else
        {
          lcd.noCursor();
          lcd.clear();
          lcd.home();
          lcd.print("Wrong Password!");
    
          if (Hcount > 0)
            {
              Hcount = Hcount - 1;
            }
    
          if (Mcount > 0)
            {
              Mcount = Mcount - 59;
            }
          if (Scount > 0)
            {
              Scount = Scount - 59;
            }
        delay(1500);
        currentLength = 0;
  
        }
      }  
    }
    //solving joystick
      if (stage == 3) {

    if (Axisjoy == 3|1|4|6) {
      delay(500);
      for (int i = 0; i < 5; i++) {
        if (output[i] == 0) {

          output[i] = Axisjoy;

          Serial.print("output "); Serial.print(i); Serial.print(" is ");
          Serial.println(output[i]);
          break;

        }
      }
    }
    if (output[4] != 0) {
      stage = 4;

      Serial.println("stage 3 ");
    }

  }
  if (stage == 4) {
    
    for (int i = 0; i < 5; i++) {
        if (input[i] == output[i]) {
          Serial.print("this is i ");
          Serial.println(i);
          winning++;
          Serial.println(winning);
        } else {
          Serial.println("we fucking lost");
          stage = 4;
          Serial.println("stage 4");
        }
    
      if (winning ==5) {
          Serial.println(" vi von ");
          stage =5;
      }
    }
}
}

void timer()
{
  Serial.print(Scount);
  Serial.println();
  
  if (Hcount <= 0)
  {
    if ( Mcount < 0 )
    {
      lcd.noCursor();
      lcd.clear();
      lcd.home();
      lcd.print("The Bomb Has ");
      lcd.setCursor (0,1);
      lcd.print("Exploded!");
      
      while (Mcount < 0) 
      {
        digitalWrite(ledPin, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin2, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin2, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin3, HIGH); // sets the LED on
        tone1.play(NOTE_A2, 90);
        delay(100); 
        digitalWrite(ledPin3, LOW); // sets the LED off
        tone1.play(NOTE_A2, 90);
        delay(100);
      }
    } 
  }

  lcd.setCursor (0,1); // sets cursor to 2nd line
  lcd.print ("Timer:");

  if (Hcount >= 10)
    {
      lcd.setCursor (7,1);
      lcd.print (Hcount);
    }
  if (Hcount < 10) 
    {
      lcd.setCursor (7,1);
      lcd.write ("0");
      lcd.setCursor (8,1);
      lcd.print (Hcount);
    }

  lcd.print (":");

  if (Mcount >= 10)
    {
      lcd.setCursor (10,1);
      lcd.print (Mcount);
    }
  if (Mcount < 10) 
    {
      lcd.setCursor (10,1);
      lcd.write ("0");
      lcd.setCursor (11,1);
      lcd.print (Mcount);
    }
    
  lcd.print (":");

  if (Scount >= 10) 
    {
      lcd.setCursor (13,1);
      lcd.print (Scount);
    }
  if (Scount < 10) 
    {
      lcd.setCursor (13,1);
      lcd.write ("0");
      lcd.setCursor (14,1);
      lcd.print (Scount);
    }

  if (Hcount <0) 
    {
      Hcount = 0; 
    }

  if (Mcount <0) 
    {
      Hcount --; 
      Mcount = 59; 
    }

  if (Scount <1) // if 60 do this operation
    {
      Mcount --; // add 1 to Mcount
      Scount = 59; // reset Scount
    }

  if (Scount > 0) // do this oper. 59 times
    {
      unsigned long currentMillis = millis();
  
      if(currentMillis - secMillis > interval) 
        {
          tone1.play(NOTE_G5, 200);
          secMillis = currentMillis;
          Scount --; // add 1 to Scount
          digitalWrite(ledPin2, HIGH); // sets the LED on
          delay(10); // waits for a second
          digitalWrite(ledPin2, LOW); // sets the LED off
          delay(10); // waits for a second
          //lcd.clear();
        }
    }
}
