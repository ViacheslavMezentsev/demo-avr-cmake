#include <stdio.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <iterator>
#include <vector>
#include <stdint.h>
#include "etl/string.h"
#include "etl/crc16_modbus.h"

extern void USART0Init();

#define F(s)    PSTR(s)

etl::string<16> fmt( "%s: 0x%04X\n" );
etl::string<16> data( "123456789" );

void test_crc16_modbus()
{
    uint16_t crc = etl::crc16_modbus_t16( data.begin(), data.end() );

    printf( fmt.c_str(), data.c_str(), crc );
}


void test_crc16_modbus_add_values()
{
    etl::crc16_modbus crc_calculator;

    for ( size_t i = 0UL; i < data.size(); ++i )
    {
        crc_calculator.add( data[i] );
    }

    uint16_t crc = crc_calculator;

    printf( fmt.c_str(), data.c_str(), crc );
}


void test_crc16_modbus_add_range()
{
    etl::crc16_modbus crc_calculator;

    crc_calculator.add( data.begin(), data.end() );

    uint16_t crc = crc_calculator.value();

    printf( fmt.c_str(), data.c_str(), crc );
}


void test_crc16_modbus_add_range_via_iterator()
{
    std::vector<uint8_t>  data1 = { 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U };
    std::vector<uint32_t> data2 = { 0x04030201UL, 0x08070605UL };
    std::vector<uint8_t>  data3 = { 0x08U, 0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U };

    uint16_t crc1 = etl::crc16_modbus (data1.begin(), data1.end() );
    printf( fmt.c_str(), data.c_str(), crc1 );

    uint16_t crc2 = etl::crc16_modbus( ( uint8_t * ) & data2[0], ( uint8_t * ) ( & data2[0] + data2.size() ) );
    printf( fmt.c_str(), data.c_str(), crc2 );

    uint16_t crc3 = etl::crc16_modbus( data3.rbegin(), data3.rend() );
    printf( fmt.c_str(), data.c_str(), crc3 );
}


void test_crc16_modbus_16()
{
    uint16_t crc = etl::crc16_modbus_t16( data.begin(), data.end() );

    printf( fmt.c_str(), data.c_str(), crc );
}


/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    // Настройка USART0.
    USART0Init();

    test_crc16_modbus();
    test_crc16_modbus_add_values();
    test_crc16_modbus_add_range();
    test_crc16_modbus_add_range_via_iterator();
    test_crc16_modbus_16();
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
}


/**
 * @brief   Точка входа.
 * 
 */
int main( void )
{
    ATOMIC_BLOCK( ATOMIC_FORCEON )
    {
        setup();
    }

    while (1)
    {
        loop();
    }
}
