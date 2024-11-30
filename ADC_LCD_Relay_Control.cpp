#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCDI2C.h"

// bit manipulation macros
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#ifndef sbi
#define sbi(reg, bit) (reg |= (_BV(bit)))
#endif

#ifndef cbi
#define cbi(reg, bit) (reg &= ~(_BV(bit)))
#endif

#ifndef tbi
#define tbi(reg, bit) (reg ^= (_BV(bit)))
#endif

void timerInit();
void externalInterruptInit();
void ADC_init();
uint16_t adcChannel0();
uint16_t adcChannel1();

volatile uint32_t time1 = 0;

int main() {
    timerInit();
    externalInterruptInit();
    ADC_init();
    sei();  // enable global interrupts

    sbi(DDRD, PD4);    // set PD4 as output
    cbi(PORTD, PD4);   // turn off relay

    LCD_I2C lcd(0x27); // initialize LCD at I2C address 0x27
    _delay_ms(100);
    lcd.backLightOn();

    char text1[20];
    char text2[20];

    while (1) {
        lcd.clear();

        uint16_t adcValue0 = adcChannel0();
        float voltage0 = adcValue0 * 0.00488;
        int voltageInt0 = voltage0 * 100;

        uint16_t adcValue1 = adcChannel1();
        float voltage1 = adcValue1 * 0.00488;
        int voltageInt1 = voltage1 * 100;

        // display voltages on LCD
        lcd.goTo(0, 0);
        sprintf(text1, "V0:%d.%02dV V1:%d.%02dV",
            voltageInt0 / 100, voltageInt0 % 100,
            voltageInt1 / 100, voltageInt1 % 100);
        lcd.writeText(text1);

        // control relay based on ADC values
        if (adcValue0 < adcValue1 + 20) {
            sbi(PORTD, PD4);  // turn on relay
            lcd.goTo(0, 1);
            sprintf(text2, "Ciemno");
            lcd.writeText(text2);
        }
        else if (adcValue0 > adcValue1 - 20) {
            cbi(PORTD, PD4);  // turn off relay
            lcd.goTo(0, 1);
            sprintf(text2, "Jasno");
            lcd.writeText(text2);
        }

        _delay_ms(50);
    }
}

void timerInit() {
    // Timer1 CTC mode with TOP at OCR1A
    cbi(TCCR1B, WGM13);
    sbi(TCCR1B, WGM12);
    cbi(TCCR1A, WGM11);
    cbi(TCCR1A, WGM10);

    // prescaler set to 256
    sbi(TCCR1B, CS12);
    cbi(TCCR1B, CS11);
    cbi(TCCR1B, CS10);

    // set OCR1A for 1-second interval
    OCR1A = 62500;

    // enable Timer1 compare interrupt
    sbi(TIMSK1, OCIE1A);
}

void externalInterruptInit() {
    // configure INT0 and INT1 for falling edge trigger
    sbi(EICRA, ISC01);
    cbi(EICRA, ISC00);
    sbi(EICRA, ISC11);
    cbi(EICRA, ISC10);

    // enable INT0 and INT1 interrupts
    sbi(EIMSK, INT0);
    sbi(EIMSK, INT1);
}

void ADC_init() {
    // reference voltage AVcc
    sbi(ADMUX, REFS0);
    cbi(ADMUX, REFS1);

    // ADC prescaler set to 128
    sbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS2);

    // enable ADC
    sbi(ADCSRA, ADEN);
}

uint16_t adcChannel0() {
    // select ADC Channel 0
    ADMUX &= 0xF0;

    // start conversion
    sbi(ADCSRA, ADSC);

    // wait for conversion to complete
    while (ADCSRA & (1 << ADSC));

    return ADC;
}

uint16_t adcChannel1() {
    // select ADC Channel 1
    ADMUX = (ADMUX & 0xF0) | 0x01;

    // start conversion
    sbi(ADCSRA, ADSC);

    // wait for conversion to complete
    while (ADCSRA & (1 << ADSC));

    return ADC;
}

// Timer1 Compare Match A Interrupt Service Routine
ISR(TIMER1_COMPA_vect) {
    time1++;
}

// External Interrupt 0 Service Routine
ISR(INT0_vect) {
    // handle interrupt
}

// External Interrupt 1 Service Routine
ISR(INT1_vect) {
    // handle interrupt
}
