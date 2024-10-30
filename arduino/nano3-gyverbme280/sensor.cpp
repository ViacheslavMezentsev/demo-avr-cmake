#include "Streaming.h"
#include "GyverBME280.h"

GyverBME280 bme;

void setup()
{
    Serial.begin( 115200 );
    Serial.println( "Старт" );

    // запуск датчика и проверка на работоспособность
    if ( !bme.begin( 0x76 ) )
    {
        Serial.println( "Ошибка!" );
    }
}

void loop()
{
    Serial << "Температура: " << bme.readTemperature() << endl;
    Serial << "Влажность: " << bme.readHumidity() << endl;
    Serial << "Давление: " << bme.readPressure() << endl << endl;

    delay( 1000 );
}