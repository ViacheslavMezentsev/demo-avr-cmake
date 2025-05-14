#include <SrcWrapper.h>
#include <USBDevice.h>
#include <Wire.h>
#include <ros.h>
#include <std_msgs/Float32.h>
#include "HMC5883L_Simple.h"

bool available = false;
unsigned long publisher_timer = 0;

HMC5883L_Simple Compass;

std_msgs::Float32 heading;
ros::Publisher pub_magn( "blackpill/heading", &heading );
ros::NodeHandle nh;

void i2cdetect()
{
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        // Проверка каждого адреса
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.print(address, HEX);
            Serial.print(" ");
            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print("-- ");
        }
        else
        {
            Serial.print(".. ");
        }

        if (address % 16 == 15)
        {
            Serial.println();
        }
        delay(10); // Небольшая задержка между проверками
    }

    if (nDevices == 0)
    {
        Serial.println("No I2C devices found");
    }
    else
    {
        Serial.println("done");
    }
}

void setup()
{
    // Настраиваем вывод светодиода.
    pinMode( LED_BUILTIN, OUTPUT );

    // Проверка доступности датчика.
    available = true;

    // Инициализация I2C.
    Wire.begin();

    Serial.begin( 9600 ); 
    Serial.println( "Scanning I2C bus..." );
    Serial.println( "   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F" );
/*
    // Magnetic Declination is the correction applied according to your present location
    // in order to get True North from Magnetic North, it varies from place to place.
    // 
    // The declination for your area can be obtained from http://www.magnetic-declination.com/
    // Take the "Magnetic Declination" line that it gives you in the information, 
    //
    // Examples:
    //   Christchurch, 23° 35' EAST
    //   Wellington  , 22° 14' EAST
    //   Dunedin     , 25° 8'  EAST
    //   Auckland    , 19° 30' EAST
    //   Екатеринбург, 15° 31' EAST
    Compass.SetDeclination( 15, 31, 'E' );

    // The device can operate in SINGLE (default) or CONTINUOUS mode
    //   SINGLE simply means that it takes a reading when you request one
    //   CONTINUOUS means that it is always taking readings
    // for most purposes, SINGLE is what you want.
    Compass.SetSamplingMode( COMPASS_SINGLE );

    // The scale can be adjusted to one of several levels, you can probably leave it at the default.
    // Essentially this controls how sensitive the device is.
    //   Options are 088, 130 (default), 190, 250, 400, 470, 560, 810
    // Specify the option as COMPASS_SCALE_xxx
    // Lower values are more sensitive, higher values are less sensitive.
    // The default is probably just fine, it works for me.  If it seems very noisy
    // (jumping around), incrase the scale to a higher one.
    Compass.SetScale( COMPASS_SCALE_130) ;

    // The compass has 3 axes, but two of them must be close to parallel to the earth's surface to read it, 
    // (we do not compensate for tilt, that's a complicated thing) - just like a real compass has a floating 
    // needle you can imagine the digital compass does too.
    //
    // To allow you to mount the compass in different ways you can specify the orientation:
    //   COMPASS_HORIZONTAL_X_NORTH (default), the compass is oriented horizontally, top-side up. when pointing North the X silkscreen arrow will point North
    //   COMPASS_HORIZONTAL_Y_NORTH, top-side up, Y is the needle,when pointing North the Y silkscreen arrow will point North
    //   COMPASS_VERTICAL_X_EAST,    vertically mounted (tall) looking at the top side, when facing North the X silkscreen arrow will point East
    //   COMPASS_VERTICAL_Y_WEST,    vertically mounted (wide) looking at the top side, when facing North the Y silkscreen arrow will point West  
    Compass.SetOrientation( COMPASS_HORIZONTAL_X_NORTH );

    // Программирование робототехники с Arduino и ROS:
    // https://habr.com/ru/articles/249401/
    nh.initNode();

    // Перенастраиваем порт.
    // Доступные скорости зависят от типа платы и частоты кварца.
    nh.getHardware()->setBaud( 115'200 );
    nh.getHardware()->init();

    nh.advertise( pub_magn );
    
    publisher_timer = millis();
*/
}


void loop()
{
    // Ожидаем срабатывания таймера.    
    if ( millis() < publisher_timer ) return;

/*
    // Публикуем, если датчик доступен.
    if ( available )
    {
        // Магнетометр.
        heading.data = Compass.GetHeadingDegrees();

        pub_magn.publish( &heading );
    }

    // Настраиваем время срабатывания таймера.
    publisher_timer = millis() + 1000;

    nh.spinOnce();
*/
    // Переключаем светодиод в темпе срабатывания таймера.
    digitalWrite( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
}
