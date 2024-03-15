#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define Password_Lenght 7 //EN: Give enough room for 6 chars + NULL char. ES: Da suficiente espacio para 6 char + NULL char.

int pos = 0; //EN: Variable to store the servo position. ES: Variable para guardar la posicion del servo.

char Data[Password_Lenght]; //EN: 6 is the number of chars it can hold + the null char = 7. ES: 6 es el numero de char que puede tener + el NULL char = 7.
char Master[Password_Lenght] = "192837"; //EN: Change PASSWORD here. ES: Cambia la CONTRASEÑA aqui.
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
bool door = true;

byte rowPins[ROWS] = { 8, 7, 6, 9 };
byte colPins[COLS] = { 5, 4, 3, 2 };

//EN: Initialize an instance of class NewKeypad. 
//ES: Inicializa la instancia de la clase NewKeypad.
Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

void setup() {
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  myservo.attach(10);
  ServoClose();
  lcd.begin(16, 2);
}

void loop() {
  if (door == 0) {
    customKey = customKeypad.getKey();
    if (customKey == '#') {  
    lcd.clear();
      lcd.setCursor (3,0);
        lcd.print("LOCKING");
        delay (700);
      lcd.setCursor (10,0);
        lcd.print(".");
        delay (800);
      lcd.setCursor (11,0);  
        lcd.print(".");
        delay (800);
      lcd.setCursor (12,0);
        lcd.print(".");
        delay (500);
        lcd.clear();
        delay (500);
        ServoClose();
      lcd.setCursor (5,0);
      lcd.print("LOCKED");
      digitalWrite(11,HIGH);
      delay(1700);
      digitalWrite(11,LOW);

      door = 1;
    }
  }
  else Open();
}

void clearData() {
  while (data_count != 0) { //EN: This can be used for any array size. ES: Se puede usar para cualquier tamaño de array.
    Data[data_count--] = 0; //EN: Clear array for new data. ES: Limpia el array para nuevo data.
  }
  return;
}
//EN: Goes from 0 degrees to 180 degrees, in steps of 5. 
//ES: Va de 0 grados a 180 grados, en pasos de 5. 
void ServoOpen() {
  for (pos = 180; pos >= 0; pos -= 5) { 
    myservo.write(pos);             
    delay(15);                       
  }
}
//EN: Goes from 180 degrees to 0 degrees, in steps of 5.
//ES: Va de 180 grados a 0 grados, en pasos de 5.
void ServoClose()
{
  for (pos = 0; pos <= 180; pos += 5) { 
    myservo.write(pos);             
    delay(15);                       
  }
}

void Open()
{
  lcd.setCursor(1, 0);
  lcd.print("INPUT PASSWORD");
  
  customKey = customKeypad.getKey();
  if (customKey) //EN: Makes sure a key is actually pressed, equal to (customKey != NO_KEY). ES: Se asugura que deverdad se toco una tecla, igual a (customKey != NO_KEY).
  {
   lcd.setCursor(0, 1); 
    lcd.print("******");     //EN: To hide your PASSWORD, make sure its the same lenght as your password. ES: Para escoder la CONTRASEÑA asegurar que tenga el mismo tamaño de ella.
    Data[data_count] = customKey; //EN: Store char into data array. ES: Guarda char en el data array.
    lcd.setCursor(data_count, 1); //EN: Move cursor to show each new char. ES: Move el cursor para eseñar cada nuevo char.
    lcd.print(Data[data_count]); //EN: Print char at said cursor. ES: Enseña el char en el dicho cursor.
    data_count++; //EN: Increment data array by 1 to store new char, also keep track of the number of chars entered. ES: Incrementa data array por 1 para guardar nuevo char, tambien sigue los numeros que fueron entrados.
  }

  if (data_count == Password_Lenght - 1) //EN: If the array index is equal to the number of expected chars, compare data to master. ES: Si el array index es igual a el numero esperado de charsm comparar el data a master.
  {
    if (!strcmp(Data, Master)) //EN: Equal to (strcmp(Data, Master) == 0). ES: es igual a (strcmp(Data, Master) == 0).
    {
      lcd.clear();
      ServoOpen();
      lcd.print(" ACCESS GRANTED");
      lcd.setCursor(0,1);
      digitalWrite(12,HIGH);
      delay(1500);
      digitalWrite(12,LOW);
      lcd.print("PRESS # TO CLOSE");
      door = 0;
    }
    else
    {
      lcd.clear();

     lcd.setCursor(1,0); 
      lcd.print("ACCESS DENIED!");
      digitalWrite(11,HIGH);
      delay(1500);
      digitalWrite(11,LOW);
      door = 1;
    }
    clearData();
  }
} 
