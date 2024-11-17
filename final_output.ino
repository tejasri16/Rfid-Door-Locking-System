//(b)Final code for the rfid tag with keypad entry using arduino
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h> // include necessary libraries for Wire (I2Ccommunication), LiquidCrystal with I2C adapter, keypad input, servo motorcontrol, MFRC522 RFID module, and SPI communication.

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER_PIN 3
#define LED_GREEN_PIN 2
#define LED_RED_PIN 1
#define SERVO_PIN 8 //declare pins for various components such as SS(Slave Select) pin for SPI communication, reset pin for the MFRC522module, pins for the buzzer, green and red LEDs, and the pin for the servomotor.

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your I2C address
Servo doorLock; // Objects are initialized for interfacing with theMFRC522 RFID module, I2C LCD display, and servo motor.

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {7,6,5,4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0,A1,A2,A3}; //connect to the column pinouts of thekeypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// Defines the layout of the keypad and initializes the Keypad object withthe specified row and column pins.

String UID = "13 F0 60 19"; // Change this to your authorized RFID tag
String correctPassword = "1245"; // Change this to your authorized
password
byte lock = 0;
int pos = 0;
// Sets the authorized RFID tag UID and the correct password forauthentication. Also initializes variables for tracking the lock state andservo motor position.

void setup() {
Serial.begin(9600);
SPI.begin();
rfid.PCD_Init();
lcd.begin(16, 2);
lcd.backlight();
doorLock.attach(SERVO_PIN);
pinMode(BUZZER_PIN, OUTPUT);
pinMode(LED_GREEN_PIN, OUTPUT);
pinMode(LED_RED_PIN, OUTPUT);
// initializes serial communication, SPI communication, the MFRC522 RFIDmodule, LCD display, servo motor, and sets initial pin modes and states.It prints a welcome message on the LCD.

// Initial state
doorLock.write(90); // Lock the door initially
lcd.print("Welcome sir,");
lcd.setCursor(0, 1);
lcd.print("card & PW");
}

void loop() {
char key = keypad.getKey();
if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Scanning");
Serial.print("NUID tag is :");
String ID = "";
for (byte i = 0; i < rfid.uid.size; i++) {
lcd.print(".");
ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
ID.concat(String(rfid.uid.uidByte[i], HEX));
delay(300);

}
ID.toUpperCase();

if (ID.substring(1) == UID && lock == 0 ) {
doorLock.write(90);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Door is locked");
buzz();
delay(2000);
lcd.clear();
lock = 1;
} else if (ID.substring(1) == UID && lock == 1 ) {
doorLock.write(180);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Door is open");
digitalWrite(LED_GREEN_PIN, HIGH);
buzz();
delay(1500);
lcd.clear();
lock = 0;
digitalWrite(LED_GREEN_PIN, LOW);
} else {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Wrong card!");


digitalWrite(LED_RED_PIN, HIGH);
buzz();
delay(2000);
lcd.clear();
digitalWrite(LED_RED_PIN, LOW);
}
lcd.clear();
rfid.PICC_HaltA();
rfid.PCD_StopCrypto1();
}

if (key && key != '#') {
static String enteredPassword = "";
enteredPassword += key;
lcd.print("*"); // Print * for each entered digit
if (enteredPassword.length() == 4) {
if (enteredPassword.equals(correctPassword)) {
lcd.clear();
lcd.print("Scanning");
delay(1000);
unlockDoor();
lcd.clear();
lcd.print("Door is Open");
digitalWrite(LED_GREEN_PIN, HIGH);
buzz();
delay(5000); // Keep door unlocked for 5 seconds
lockDoor();


digitalWrite(LED_GREEN_PIN, LOW);
} else {
lcd.clear();
lcd.print("Wrong password");
digitalWrite(LED_RED_PIN, HIGH);
buzz();
delay(3000);
digitalWrite(LED_RED_PIN, LOW);
}
enteredPassword = ""; // Reset entered password
}
}
}
//continuously checks for new RFID cards and keypad input. If a new card is present, it reads its UID and performs authentication. If a keypad keyis pressed (excluding '#'), it collects the entered password and performsauthentication.

void unlockDoor() {
doorLock.write(90); // Open the door
}

void lockDoor() {
doorLock.write(0); // Close the door
}

void buzz() {
digitalWrite(BUZZER_PIN, HIGH);
delay(500);

15

digitalWrite(BUZZER_PIN, LOW);
delay(500);
}// for unlocking the door (by opening the servo motor), locking the door(by closing the servo motor), and activating the buzzer for a shortduration.