// Mx_Basics.h

#ifndef _MX_BASICS_h
#define _MX_BASICS_h

#include "arduino.h"


/* * * * * * * * * *
 *                 *
 *  Delay-Timings  *
 *                 *
 * * * * * * * * * */

#if F_CPU > 20000

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
#define __DELAY_WAIT_1MS__ (F_CPU / 5000 - 3)
#endif

#endif

#if defined(__DELAY_WAIT_1MS__)
void delayDirect(uint16_t ms);
#else
 // --- vanilla ---
#define VANILLA_MCU
#define delayDirect(ms) delay((ms))
#endif


/* * * * * * * * * *
 *                 *
 *  Port-Mappings  *
 *                 *
 * * * * * * * * * */

#if defined(__AVR_ATmega328P__)
 // --- Arduino UNO ---
#define digitalWriteDirect(pin, val) ( (val)==0 ? ( \
  (pin)<8 ? PORTD&=~(1<<(pin)) : (pin)<14 ? PORTB&=~(1<<((pin)-8)) : PORTC&=~(1<<((pin)-14)) \
  ) : ( \
  (pin)<8 ? PORTD|=(1<<(pin)) : (pin)<14 ? PORTB|=(1<<((pin)-8)) : PORTC|=(1<<((pin)-14)) \
  ) );
#define pinModeDirect(pin, val) ( (val)==0 ? ( \
  (pin)<8 ? DDRD&=~(1<<(pin)) : (pin)<14 ? DDRB&=~(1<<((pin)-8)) : DDRC&=~(1<<((pin)-14)) \
  ) : ( \
  (pin)<8 ? DDRD|=(1<<(pin)) : (pin)<14 ? DDRB|=(1<<((pin)-8)) : DDRC|=(1<<((pin)-14)) \
  ) );
#elif defined(__AVR_ATmega2560__)
 // --- Arduino MEGA ---
#define digitalWriteDirect(pin, val) ( (val)==0 ? ( \
  (pin)<2 ? PORTE&=~(1<<(pin)) : (pin)<4 ? PORTE&=~(1<<((pin)+2)) : (pin)<5 ? PORTG&=~(1<<((pin)+1)) : (pin)<6 ? PORTE&=~(1<<((pin)-2)) : (pin)<10 ? PORTH&=~(1<<((pin)-3)) : (pin)<14 ? PORTB&=~(1<<((pin)-6)) : \
  (pin)<15 ? PORTJ&=~(1<<((pin)-13)) : (pin)<16 ? PORTJ&=~(1<<((pin)-15)) : (pin)<17 ? PORTH&=~(1<<((pin)-15)) : (pin)<18 ? PORTH&=~(1<<((pin)-17)) : (pin)<19 ? PORTD&=~(1<<((pin)-15)) : \
  (pin)<20 ? PORTD&=~(1<<((pin)-17)) : (pin)<21 ? PORTD&=~(1<<((pin)-19)) : (pin)<22 ? PORTD&=~(1<<((pin)-21)) : (pin)<30 ? PORTA&=~(1<<((pin)-22)) : (pin)<31 ? PORTC&=~(1<<((pin)-23)) : \
  (pin)<32 ? PORTC&=~(1<<((pin)-25)) : (pin)<33 ? PORTC&=~(1<<((pin)-27)) : (pin)<34 ? PORTC&=~(1<<((pin)-29)) : (pin)<35 ? PORTC&=~(1<<((pin)-31)) : (pin)<36 ? PORTC&=~(1<<((pin)-33)) : \
  (pin)<37 ? PORTC&=~(1<<((pin)-35)) : (pin)<38 ? PORTC&=~(1<<((pin)-37)) : (pin)<39 ? PORTD&=~(1<<((pin)-31)) : (pin)<40 ? PORTG&=~(1<<((pin)-37)) : (pin)<41 ? PORTG&=~(1<<((pin)-39)) : \
  (pin)<42 ? PORTG&=~(1<<((pin)-41)) : (pin)<43 ? PORTL&=~(1<<((pin)-35)) : (pin)<44 ? PORTL&=~(1<<((pin)-37)) : (pin)<45 ? PORTL&=~(1<<((pin)-39)) : (pin)<46 ? PORTL&=~(1<<((pin)-41)) : \
  (pin)<47 ? PORTL&=~(1<<((pin)-43)) : (pin)<48 ? PORTL&=~(1<<((pin)-45)) : (pin)<49 ? PORTL&=~(1<<((pin)-47)) : (pin)<50 ? PORTL&=~(1<<((pin)-49)) : (pin)<51 ? PORTB&=~(1<<((pin)-47)) : \
  (pin)<52 ? PORTB&=~(1<<((pin)-49)) : (pin)<53 ? PORTB&=~(1<<((pin)-51)) : (pin)<54 ? PORTB&=~(1<<((pin)-53)) : (pin)<62 ? PORTF&=~(1<<((pin)-54)) : PORTK&=~(1<<((pin)-62)) \
  ) : ( \
  (pin)<2 ? PORTE|=(1<<(pin)) : (pin)<4 ? PORTE|=(1<<((pin)+2)) : (pin)<5 ? PORTG|=(1<<((pin)+1)) : (pin)<6 ? PORTE|=(1<<((pin)-2)) : (pin)<10 ? PORTH|=(1<<((pin)-3)) : (pin)<14 ? PORTB|=(1<<((pin)-6)) : \
  (pin)<15 ? PORTJ|=(1<<((pin)-13)) : (pin)<16 ? PORTJ|=(1<<((pin)-15)) : (pin)<17 ? PORTH|=(1<<((pin)-15)) : (pin)<18 ? PORTH|=(1<<((pin)-17)) : (pin)<19 ? PORTD|=(1<<((pin)-15)) : \
  (pin)<20 ? PORTD|=(1<<((pin)-17)) : (pin)<21 ? PORTD|=(1<<((pin)-19)) : (pin)<22 ? PORTD|=(1<<((pin)-21)) : (pin)<30 ? PORTA|=(1<<((pin)-22)) : (pin)<31 ? PORTC|=(1<<((pin)-23)) : \
  (pin)<32 ? PORTC|=(1<<((pin)-25)) : (pin)<33 ? PORTC|=(1<<((pin)-27)) : (pin)<34 ? PORTC|=(1<<((pin)-29)) : (pin)<35 ? PORTC|=(1<<((pin)-31)) : (pin)<36 ? PORTC|=(1<<((pin)-33)) : \
  (pin)<37 ? PORTC|=(1<<((pin)-35)) : (pin)<38 ? PORTC|=(1<<((pin)-37)) : (pin)<39 ? PORTD|=(1<<((pin)-31)) : (pin)<40 ? PORTG|=(1<<((pin)-37)) : (pin)<41 ? PORTG|=(1<<((pin)-39)) : \
  (pin)<42 ? PORTG|=(1<<((pin)-41)) : (pin)<43 ? PORTL|=(1<<((pin)-35)) : (pin)<44 ? PORTL|=(1<<((pin)-37)) : (pin)<45 ? PORTL|=(1<<((pin)-39)) : (pin)<46 ? PORTL|=(1<<((pin)-41)) : \
  (pin)<47 ? PORTL|=(1<<((pin)-43)) : (pin)<48 ? PORTL|=(1<<((pin)-45)) : (pin)<49 ? PORTL|=(1<<((pin)-47)) : (pin)<50 ? PORTL|=(1<<((pin)-49)) : (pin)<51 ? PORTB|=(1<<((pin)-47)) : \
  (pin)<52 ? PORTB|=(1<<((pin)-49)) : (pin)<53 ? PORTB|=(1<<((pin)-51)) : (pin)<54 ? PORTB|=(1<<((pin)-53)) : (pin)<62 ? PORTF|=(1<<((pin)-54)) : PORTK|=(1<<((pin)-62)) \
  ) );
#define pinModeDirect(pin, val) ( (val)==0 ? ( \
  (pin)<2 ? DDRE&=~(1<<(pin)) : (pin)<4 ? DDRE&=~(1<<((pin)+2)) : (pin)<5 ? DDRG&=~(1<<((pin)+1)) : (pin)<6 ? DDRE&=~(1<<((pin)-2)) : (pin)<10 ? DDRH&=~(1<<((pin)-3)) : (pin)<14 ? DDRB&=~(1<<((pin)-6)) : \
  (pin)<15 ? DDRJ&=~(1<<((pin)-13)) : (pin)<16 ? DDRJ&=~(1<<((pin)-15)) : (pin)<17 ? DDRH&=~(1<<((pin)-15)) : (pin)<18 ? DDRH&=~(1<<((pin)-17)) : (pin)<19 ? DDRD&=~(1<<((pin)-15)) : \
  (pin)<20 ? DDRD&=~(1<<((pin)-17)) : (pin)<21 ? DDRD&=~(1<<((pin)-19)) : (pin)<22 ? DDRD&=~(1<<((pin)-21)) : (pin)<30 ? DDRA&=~(1<<((pin)-22)) : (pin)<31 ? DDRC&=~(1<<((pin)-23)) : \
  (pin)<32 ? DDRC&=~(1<<((pin)-25)) : (pin)<33 ? DDRC&=~(1<<((pin)-27)) : (pin)<34 ? DDRC&=~(1<<((pin)-29)) : (pin)<35 ? DDRC&=~(1<<((pin)-31)) : (pin)<36 ? DDRC&=~(1<<((pin)-33)) : \
  (pin)<37 ? DDRC&=~(1<<((pin)-35)) : (pin)<38 ? DDRC&=~(1<<((pin)-37)) : (pin)<39 ? DDRD&=~(1<<((pin)-31)) : (pin)<40 ? DDRG&=~(1<<((pin)-37)) : (pin)<41 ? DDRG&=~(1<<((pin)-39)) : \
  (pin)<42 ? DDRG&=~(1<<((pin)-41)) : (pin)<43 ? DDRL&=~(1<<((pin)-35)) : (pin)<44 ? DDRL&=~(1<<((pin)-37)) : (pin)<45 ? DDRL&=~(1<<((pin)-39)) : (pin)<46 ? DDRL&=~(1<<((pin)-41)) : \
  (pin)<47 ? DDRL&=~(1<<((pin)-43)) : (pin)<48 ? DDRL&=~(1<<((pin)-45)) : (pin)<49 ? DDRL&=~(1<<((pin)-47)) : (pin)<50 ? DDRL&=~(1<<((pin)-49)) : (pin)<51 ? DDRB&=~(1<<((pin)-47)) : \
  (pin)<52 ? DDRB&=~(1<<((pin)-49)) : (pin)<53 ? DDRB&=~(1<<((pin)-51)) : (pin)<54 ? DDRB&=~(1<<((pin)-53)) : (pin)<62 ? DDRF&=~(1<<((pin)-54)) : DDRK&=~(1<<((pin)-62)) \
  ) : ( \
  (pin)<2 ? DDRE|=(1<<(pin)) : (pin)<4 ? DDRE|=(1<<((pin)+2)) : (pin)<5 ? DDRG|=(1<<((pin)+1)) : (pin)<6 ? DDRE|=(1<<((pin)-2)) : (pin)<10 ? DDRH|=(1<<((pin)-3)) : (pin)<14 ? DDRB|=(1<<((pin)-6)) : \
  (pin)<15 ? DDRJ|=(1<<((pin)-13)) : (pin)<16 ? DDRJ|=(1<<((pin)-15)) : (pin)<17 ? DDRH|=(1<<((pin)-15)) : (pin)<18 ? DDRH|=(1<<((pin)-17)) : (pin)<19 ? DDRD|=(1<<((pin)-15)) : \
  (pin)<20 ? DDRD|=(1<<((pin)-17)) : (pin)<21 ? DDRD|=(1<<((pin)-19)) : (pin)<22 ? DDRD|=(1<<((pin)-21)) : (pin)<30 ? DDRA|=(1<<((pin)-22)) : (pin)<31 ? DDRC|=(1<<((pin)-23)) : \
  (pin)<32 ? DDRC|=(1<<((pin)-25)) : (pin)<33 ? DDRC|=(1<<((pin)-27)) : (pin)<34 ? DDRC|=(1<<((pin)-29)) : (pin)<35 ? DDRC|=(1<<((pin)-31)) : (pin)<36 ? DDRC|=(1<<((pin)-33)) : \
  (pin)<37 ? DDRC|=(1<<((pin)-35)) : (pin)<38 ? DDRC|=(1<<((pin)-37)) : (pin)<39 ? DDRD|=(1<<((pin)-31)) : (pin)<40 ? DDRG|=(1<<((pin)-37)) : (pin)<41 ? DDRG|=(1<<((pin)-39)) : \
  (pin)<42 ? DDRG|=(1<<((pin)-41)) : (pin)<43 ? DDRL|=(1<<((pin)-35)) : (pin)<44 ? DDRL|=(1<<((pin)-37)) : (pin)<45 ? DDRL|=(1<<((pin)-39)) : (pin)<46 ? DDRL|=(1<<((pin)-41)) : \
  (pin)<47 ? DDRL|=(1<<((pin)-43)) : (pin)<48 ? DDRL|=(1<<((pin)-45)) : (pin)<49 ? DDRL|=(1<<((pin)-47)) : (pin)<50 ? DDRL|=(1<<((pin)-49)) : (pin)<51 ? DDRB|=(1<<((pin)-47)) : \
  (pin)<52 ? DDRB|=(1<<((pin)-49)) : (pin)<53 ? DDRB|=(1<<((pin)-51)) : (pin)<54 ? DDRB|=(1<<((pin)-53)) : (pin)<62 ? DDRF|=(1<<((pin)-54)) : DDRK|=(1<<((pin)-62)) \
  ) );
#else
 // --- vanilla ---
#define VANILLA_MCU
#define digitalWriteDirect(pin, val) digitalWrite((pin), (val))
#define pinModeDirect(pin, val) pinMode((pin), (val))
#endif

#define swapInt(val1, val2) { int tmp = val1; val1 = val2; val2 = tmp; }

#define color565(r, g, b) (((r) & 0xF8) << 8 | ((g) & 0xFC) << 3 | (b) >> 3)

#endif
