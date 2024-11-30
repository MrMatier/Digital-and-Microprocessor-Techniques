#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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

// lookup table for 7-segment display encoding
uint8_t segment_codes[10] = {
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01101111  //9
};

void timerInit();
void externalInterruptInit();

volatile int counter = 1234;
volatile int display_position = 1;

int main() {
    timerInit();
    externalInterruptInit();
    sei();  // enable global interrupts

    DDRD = 0xFF;   // PORTD as output
    DDRC = 0xFF;   // PORTC as output
    DDRB = 0x00;   // PORTB as input
    PORTB = 0xFF;  // enable pull-up resistors

    while (1) {
        // main loop
    }
}

void timerInit() {
    // set Timer1 to CTC mode with TOP at OCR1A
    cbi(TCCR1B, WGM13);
    sbi(TCCR1B, WGM12);
    cbi(TCCR1A, WGM11);
    cbi(TCCR1A, WGM10);

    // prescaler set to 256
    sbi(TCCR1B, CS12);
    cbi(TCCR1B, CS11);
    cbi(TCCR1B, CS10);

    // set OCR1A for desired interrupt frequency
    OCR1A = 200;

    // enable Timer1 compare interrupt
    sbi(TIMSK1, OCIE1A);
}

void externalInterruptInit() {
    // configure INT0 for falling edge trigger
    sbi(EICRA, ISC01);
    cbi(EICRA, ISC00);

    // enable INT0 interrupt
    sbi(EIMSK, INT0);
}

// Timer1 Compare Match A Interrupt Service Routine
ISR(TIMER1_COMPA_vect) {
    int x = counter;
    switch (display_position) {
    case 1:
        cbi(PORTC, PC3);
        sbi(PORTC, PC0);
        PORTD = (x < 1000) ? segment_codes[0] : segment_codes[x / 1000];
        display_position++;
        break;
    case 2:
        cbi(PORTC, PC0);
        sbi(PORTC, PC1);
        PORTD = (x < 100) ? segment_codes[0] : segment_codes[(x / 100) % 10];
        display_position++;
        break;
    case 3:
        cbi(PORTC, PC1);
        sbi(PORTC, PC2);
        PORTD = (x < 10) ? segment_codes[0] : segment_codes[(x / 10) % 10];
        display_position++;
        break;
    case 4:
        cbi(PORTC, PC2);
        sbi(PORTC, PC3);
        PORTD = segment_codes[x % 10];
        display_position = 1;
        break;
    default:
        display_position = 1;
        break;
    }
}

// External Interrupt 0 Service Routine
ISR(INT0_vect) {
    // handle external interrupt
}
