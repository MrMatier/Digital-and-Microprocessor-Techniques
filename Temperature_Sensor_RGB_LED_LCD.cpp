#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int tempSensorPin = A0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setColor(int red, int green, int blue) {
    analogWrite(redPin, 255 - red);
    analogWrite(greenPin, 255 - green);
    analogWrite(bluePin, 255 - blue);
}

void setup() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(tempSensorPin, INPUT);
    lcd.init();
    lcd.backlight();
}

void loop() {
    int sensorValue = analogRead(tempSensorPin);
    float voltage = sensorValue * 0.00488;    // convert to voltage
    float temperature = voltage * 20;         // example conversion to temperature

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V:");
    lcd.print(voltage, 2);
    lcd.print("V");
    lcd.setCursor(8, 0);
    lcd.print("T:");
    lcd.print(temperature, 1);
    lcd.print("C");

    // set RGB LED color based on temperature
    if (temperature < 25) {
        setColor(0, 255, 0);  // green
        lcd.setCursor(0, 1);
        lcd.print("<25C - Cold");
    }
    else if (temperature < 50) {
        setColor(0, 0, 255);  // blue
        lcd.setCursor(0, 1);
        lcd.print("25-50C - Warm");
    }
    else if (temperature < 75) {
        setColor(0, 123, 255);  // light blue
        lcd.setCursor(0, 1);
        lcd.print("50-75C - Hot");
    }
    else if (temperature < 90) {
        setColor(255, 0, 255);  // purple
        lcd.setCursor(0, 1);
        lcd.print("75-90C - Boil");
    }
    else {
        setColor(255, 0, 0);  // red
        lcd.setCursor(0, 1);
        lcd.print(">90C - Burn");
    }

    delay(1000);
}
