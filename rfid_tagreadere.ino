#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h> // including libraries for interfacing with theI2C LCD display, SPI communication, and the MFRC522 RFID module.

#define RST_PIN 9
#define SS_PIN 10 // Declaring the pin numbers for the reset(RST_PIN) and chip select (SS_PIN) pins of the MFRC522 module.
byte readCard[4]; // an array readCard of size 4 is declared tostore the UID (unique identifier) read from the RFID card.
byte a = 0;// Variable a is initialized to 0, which will be used forLCD display positioning.

LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
Serial.begin(9600); //Serial communication is started at a baudrate of 9600.
lcd.init();
lcd.backlight(); //The LCD is initialized and its backlight isturned on.
while (!Serial); //The program waits until a serial connection isestablished.
SPI.begin();//SPI communication is initiated.
mfrc522.PCD_Init(); //The MFRC522 module is initialized.
delay(4);
mfrc522.PCD_DumpVersionToSerial(); //The version information ofthe MFRC522 module is dumped to the serial monitor.
lcd.setCursor(2, 0);
lcd.print("Put your card"); //The message "Put your card" isprinted on the LCD display at position (2,0).

}

void loop() {
if ( ! mfrc522.PICC_IsNewCardPresent()) {
return 0;
}
if ( ! mfrc522.PICC_ReadCardSerial()) {
return 0;
}
//It checks if a new RFID card is present and if it can be read.If either condition fails, the loop returns immediately.


lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Scanned UID"); // clears the LCD display and prints"Scanned UID" at the beginning of the first row.
a = 0; //
Serial.println(F("Scanned PICC's UID:"));
for ( uint8_t i = 0; i < 4; i++) { //
readCard[i] = mfrc522.uid.uidByte[i];
Serial.print(readCard[i], HEX);
Serial.print(" ");
lcd.setCursor(a, 1);
lcd.print(readCard[i], HEX);
lcd.print(" ");
delay(500);
a += 3;
} // It initializes variable a to 0 and prints the scanned UID
byte by byte on the LCD display, also printing them in hexadecimal
format on the serial monitor.

Serial.println("");
mfrc522.PICC_HaltA(); //After printing, it halts the RFID reader
return 1; //It then returns 1. However, the return value of 1doesn't have any effect here as the loop function is declared as
void.

}

