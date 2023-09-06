#include "Defines.h"
#include "Logger.h"


size_t CLogger::write( uint8_t ch )
{
    // Ждём окончания передачи предыдущего байта.
    while ( !CHECKBIT( UCSR0A, UDRE0 ) );

    UDR0 = ch;

    // Ожидаем окончания передачи.
    while ( !CHECKBIT( UCSR0A, TXC0 ) );

    // Очищаем биты.
    UCSR0A &= ~( _BV( TXC0 ) | _BV( UDRE0 ) );

    return 1;
}


size_t CLogger::println( char * format, ... )
{
    // Буфер используется как для входной строки,
    // так и для хранения результата.
    char * ptr;

    // Выводим сообщение.
    // Копируем из памяти в буфер.
    strncpy( _buffer, format, sizeof _buffer );

    // null terminate - leave last char since we might need 
    // it in worst case for result's \0
    _buffer[ sizeof _buffer - 2 ] = '\0';

    // Указатель результата.
    ptr = & _buffer[ strlen( _buffer ) + 1 ];

    va_list args;

    va_start( args, format );
    vsnprintf( ptr, sizeof _buffer - 1 - strlen( _buffer ), _buffer, args );
    va_end( args );

    _buffer[ sizeof _buffer - 1 ] = '\0';

    size_t n = Print::write( ( const char * ) ptr );
    n += write( '\r' );
    n += write( '\n' );

    return n;
}


size_t CLogger::println( PGM_P format, ... )
{
    // Буфер используется как для входной строки,
    // так и для хранения результата.
    char * ptr;

    // Выводим сообщение.
    // Копируем из памяти в буфер.
    strncpy_P( _buffer, format, sizeof _buffer );

    // null terminate - leave last char since we might need 
    // it in worst case for result's \0
    _buffer[ sizeof _buffer - 2 ] = '\0';

    // Указатель результата.
    ptr = & _buffer[ strlen( _buffer ) + 1 ];

    va_list args;

    va_start( args, format );
    vsnprintf( ptr, sizeof _buffer - 1 - strlen( _buffer ), _buffer, args );
    va_end( args );

    _buffer[ sizeof _buffer - 1 ] = '\0';

    size_t n = Print::write( ( const char * ) ptr );
    n += write( '\r' );
    n += write( '\n' );

    return n;
}
