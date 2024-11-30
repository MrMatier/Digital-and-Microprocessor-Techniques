#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

// bit manipulation macros
#ifndef bit_is_set
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#endif

#ifndef bit_is_clear
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))
#endif

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

// function to display a number on a 4-digit 7-segment display
void display7(int number) {
    // thousands place
    sbi(PORTC, PC0);
    if (number < 1000)
        PORTD = segment_codes[0];
    else
        PORTD = segment_codes[number / 1000];
    _delay_ms(5);
    cbi(PORTC, PC0);

    // hundreds place
    sbi(PORTC, PC1);
    if (number < 100)
        PORTD = segment_codes[0];
    else
        PORTD = segment_codes[(number / 100) % 10];
    _delay_ms(5);
    cbi(PORTC, PC1);

    // tens place
    sbi(PORTC, PC2);
    if (number < 10)
        PORTD = segment_codes[0];
    else
        PORTD = segment_codes[(number / 10) % 10];
    _delay_ms(5);
    cbi(PORTC, PC2);

    // ones place
    sbi(PORTC, PC3);
    PORTD = segment_codes[number % 10];
    _delay_ms(5);
    cbi(PORTC, PC3);
}

int main() {
    DDRD = 0xFF;   // set PORTD as output
    DDRC = 0xFF;   // set PORTC as output
    DDRB = 0x00;   // set PORTB as input
    PORTB = 0xFF;  // enable pull-up resistors on PORTB

    int counter = 0;

    while (1) {
        display7(counter);

        // decrement button (SW1)
        if (bit_is_clear(PINB, PB0)) {
            while (bit_is_clear(PINB, PB0));
            if (counter > 0) {
                counter--;
            }
        }

        // increment button (SW2)
        if (bit_is_clear(PINB, PB1)) {
            while (bit_is_clear(PINB, PB1));
            if (counter < 9999) {
                counter++;
            }
        }

        // reset button (SW3)
        if (bit_is_clear(PINB, PB2)) {
            while (bit_is_clear(PINB, PB2));
            counter = 0;
        }

        // start countdown button (SW4)
        if (bit_is_clear(PINB, PB3)) {
            while (bit_is_clear(PINB, PB3));
            while (counter > 0) {
                if (bit_is_clear(PINB, PB2)) {  // reset during countdown
                    while (bit_is_clear(PINB, PB2));
                    counter = 0;
                    break;
                }
                counter--;
                for (int j = 0; j < 50; j++) {
                    display7(counter);
                }
            }
            // blink LED to indicate countdown completion
            sbi(PORTC, PC4);
            _delay_ms(50);
            cbi(PORTC, PC4);
            _delay_ms(50);
            sbi(PORTC, PC4);
            _delay_ms(50);
            cbi(PORTC, PC4);
            _delay_ms(50);
            sbi(PORTC, PC4);
            _delay_ms(50);
            cbi(PORTC, PC4);
        }
    }
}
