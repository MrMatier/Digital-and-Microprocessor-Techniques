#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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

void initUART();
void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);
void writeText(char* text);
void ADC_init();
uint16_t adcChannel0();
uint16_t adcChannel1();
void timerInit();
void externalInterruptInit();

volatile int counter = 0;

int main() {
    ADC_init();
    initUART();
    timerInit();
    externalInterruptInit();
    sei();  // enable global interrupts

    sbi(DDRD, PD4);    // set PD4 as output
    cbi(PORTD, PD4);   // turn off LED
    _delay_ms(50);

    while (1) {
        char receivedChar = USART_Receive();

        if (receivedChar == 'a') {
            sbi(PORTD, PD4);    // turn on LED
            writeText("LED ON");
        }
        else if (receivedChar == 's') {
            cbi(PORTD, PD4);    // turn off LED
            writeText("LED OFF");
        }
        else if (receivedChar == 'd') {
            if (bit_is_clear(PIND, PD4)) {
                writeText("LED OFF");
            }
            else {
                writeText("LED ON");
            }
        }
        else if (receivedChar == 'f') {
            char timeString[10];
            sprintf(timeString, "%03d", counter);
            writeText(timeString);
        }
        _delay_ms(100);
    }
}

void initUART() {
    // set baud rate to 19200 (UBRR = 51 for 16MHz clock)
    UBRR0 = 51;
    sbi(UCSR0B, RXEN0);  // enable receiver
    sbi(UCSR0B, TXEN0);  // enable transmitter
}

void USART_Transmit(unsigned char data) {
    // wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    // send data
    UDR0 = data;
}

unsigned char USART_Receive(void) {
    // wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));
    // return received data
    return UDR0;
}

void writeText(char* text) {
    while (*text)
        USART_Transmit(*text++);
    USART_Transmit('\n');
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
    // configure INT0 for falling edge trigger
    sbi(EICRA, ISC01);
    cbi(EICRA, ISC00);

    // enable INT0 interrupt
    sbi(EIMSK, INT0);
}

// Timer1 Compare Match A Interrupt Service Routine
ISR(TIMER1_COMPA_vect) {
    counter++;
}

// External Interrupt 0 Service Routine
ISR(INT0_vect) {
    // handle external interrupt
}
