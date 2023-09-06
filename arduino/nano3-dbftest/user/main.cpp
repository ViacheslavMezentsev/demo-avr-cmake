#include "Defines.h"
#include "FatFs/FatFs.h"

PR_BEGIN_EXTERN_C

#include "libdbf/dbf.h"

PR_END_EXTERN_C

#define O_READ   FA_READ
#define O_WRITE  FA_WRITE
#define O_RDWR   FA_READ | FA_WRITE
#define O_CREAT  FA_CREATE_ALWAYS

#undef FDEV_SETUP_STREAM
#define FDEV_SETUP_STREAM(p, g, f) \
    {                              \
        .buf = NULL,               \
        .unget = 0,                \
        .flags = f,                \
        .size = 0,                 \
        .len = 0,                  \
        .put = p,                  \
        .get = g,                  \
        .udata = 0                 \
    }

static int uart_putchar( char c, FILE * stream );
FILE mystdout = FDEV_SETUP_STREAM( uart_putchar, NULL, _FDEV_SETUP_WRITE );

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000UL )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

struct RECORD
{
    uint32_t time;
    uint32_t id;
    float value;
};

//CLogger Logger;
CFatFs FatFs;
CDirFs DirFs;
DB_FIELD fields[3];


/**
 * @brief   Обработчик прерывания TIMER0_OVF.
 * 
 */
ISR( TIMER0_OVF_vect )
{
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;

    if ( f >= FRACT_MAX )
    {
        f -= FRACT_MAX;
        m += 1;
    }

    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}


/**
 * @brief   Настройка таймера Timer0.
 * 
 */
void Timer0Init()
{
    // on the ATmega168, timer 0 is also used for fast hardware pwm
    // (using phase-correct PWM would mean that timer 0 overflowed half as often
    // resulting in different millis() behavior on the ATmega8 and ATmega168)
    TCCR0A = _BV( WGM01 ) | _BV( WGM00 );

    // set timer 0 prescale factor to 64
    // this combination is for the standard 168/328/1280/2560
    TCCR0B = _BV( CS01 ) | _BV( CS00 );

    // enable timer 0 overflow interrupt
    TIMSK0 |= _BV( TOIE0 );
}


/**
* Настройка внутреннего USART0
*/
void USART0Init()
{
    // Double the USART Transmission Speed.
    UCSR0A |= _BV( U2X0 );

    // Устанавливаем скорость.
    UBRR0H = BAUD_REG_VALUE >> 8;
    UBRR0L = BAUD_REG_VALUE;

    // Включаем передатчик USART0.
    UCSR0B = _BV( TXEN0 );

    // Формат передачи: 8-N-1.
    UCSR0C = _BV( UCSZ01 ) | _BV( UCSZ00 );
}


/**
 * @brief   Вывод одиночного символа в последовательный порт USART0.
 * 
 * @param c         символ 
 * @param stream    указатель на структуру FILE
 * @return int      результат
 */
int uart_putchar( char c, FILE * stream )
{    
    ( void ) stream;

    // Ждём окончания передачи предыдущего байта.
    loop_until_bit_is_set( UCSR0A, UDRE0 );

    UDR0 = c;

    // Ожидаем окончания передачи.
    loop_until_bit_is_set( UCSR0A, TXC0 );

    // Очищаем биты.
    UCSR0A &= ~( _BV( TXC0 ) | _BV( UDRE0 ) );

    return 0;
}


/**
 * @brief   Возвращает текущее количество миллисекунд.
 * 
 */
unsigned long millis()
{
    unsigned long msec;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )
    {
        msec = timer0_millis;
    }

    return msec;
}


void DBFTest()
{
    char dbfile[] = "20171007.dbf";

    CDBF::SetField( & fields[0], 'I', "TIME", 4, 0 );
    CDBF::SetField( & fields[1], 'I', "ID", 4, 0 );
    CDBF::SetField( & fields[2], 'O', "VALUE", 4, 0 );

    if ( FatFs.begin( 1, 1 ) )
    {
        printf_P( F( "SD initialized.\n" ) );

        if ( FatFs.exists( dbfile ) ) FatFs.remove( dbfile );

        auto pdbf = CDBF::Create( dbfile, fields, sizeof( fields ) / sizeof( DB_FIELD ) );

        if ( pdbf != nullptr )
        {
            printf_P( F( "File created: %s\n" ), dbfile );

            RECORD rec;

            rec.time = 0;
            rec.id = 1;
            rec.value = 1.0;

            int16_t rcount = CDBF::WriteRecord( ( char * ) & rec, sizeof( RECORD ) );

            printf_P( F( "Records %i\n" ), rcount );

            CDBF::Close();
        }
    }
    else
    {
        printf_P( F( "SD not initialized.\n" ) );
    }
}


int main()
{       
    ATOMIC_BLOCK( ATOMIC_FORCEON )
    {
        // Настройка таймера Timer0.
        Timer0Init();

        // Настройка USART0.
        USART0Init();

        sbi( DDRB, DDB5 );

        stdout = & mystdout;
    }

    DBFTest();
    
    while (1)
    {
        // Мигаем светодиодом.
        TOGGLE( PORTB, PORTB5 );

        _delay_ms( 1000 );
    }
}
