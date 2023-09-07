#include "Defines.h"
#include "FatFs/FatFs.h"

PR_BEGIN_EXTERN_C
#include "libdbf/dbf.h"
PR_END_EXTERN_C

extern void Timer0Init();
extern void USART0Init();
extern unsigned long millis();

#define O_READ   FA_READ
#define O_WRITE  FA_WRITE
#define O_RDWR   FA_READ | FA_WRITE
#define O_CREAT  FA_CREATE_ALWAYS

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


/**
 * @brief   Точка входа.
 * 
 */
int main()
{       
    ATOMIC_BLOCK( ATOMIC_FORCEON )
    {
        // Настройка таймера Timer0.
        Timer0Init();

        // Настройка USART0.
        USART0Init();

        sbi( DDRB, DDB5 );
    }

    DBFTest();
    
    while (1)
    {
        // Мигаем светодиодом.
        TOGGLE( PORTB, PORTB5 );

        _delay_ms( 1000 );
    }
}
