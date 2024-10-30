#include <SPI.h>
#include <SrcWrapper.h>
#include <USBDevice.h>
//#include <BMI270.h>
#include <ros.h>
#include <std_msgs/Float32.h>

static const uint8_t MOSI_PIN = PA7;
static const uint8_t MISO_PIN = PA6;
static const uint8_t SCLK_PIN = PA5;
static const uint8_t CS_PIN  = PA4;

static SPIClass spi = SPIClass( MOSI_PIN, MISO_PIN, SCLK_PIN );
//static BMI270 imu = BMI270( spi, CS_PIN );

bool available = false;
unsigned long publisher_timer = 0;

std_msgs::Float32 temp_msg, hum_msg, pres_msg;
ros::Publisher pub_temp( "speedybeef4v3/temperature", &temp_msg );
ros::Publisher pub_hum( "speedybeef4v3/humidity", &hum_msg );
ros::Publisher pub_pres( "speedybeef4v3/pressure", &pres_msg );
ros::NodeHandle nh;

void setup()
{
    // Настраиваем вывод светодиода.
    pinMode( LED_BUILTIN, OUTPUT );

    spi.begin();
    //imu.begin();

    // Проверка доступности датчика BMP280 (0x76) на частоте 400 кГц.
    available = true;

    // Программирование робототехники с Arduino и ROS:
    // https://habr.com/ru/articles/249401/
    nh.initNode();

    // Перенастраиваем порт.
    // Доступные скорости зависят от типа платы и частоты кварца.
    nh.getHardware()->setBaud( 2'000'000 );
    nh.getHardware()->init();

    nh.advertise( pub_temp );
    nh.advertise( pub_hum );
    nh.advertise( pub_pres );

    publisher_timer = millis();
}

void loop()
{
    // Ожидаем срабатывания таймера.    
    if ( millis() < publisher_timer ) return;

    // Публикуем, если датчик доступен.
    if ( available )
    {
        // Температура.
        temp_msg.data++;
        pub_temp.publish( &temp_msg );

        // Влажность.
        hum_msg.data++;
        pub_hum.publish( &hum_msg );

        // Давление.
        pres_msg.data++;
        pub_pres.publish( &pres_msg );
    }

    // Настраиваем время срабатывания таймера.
    publisher_timer = millis() + 1000;

    nh.spinOnce();

    // Переключаем светодиод в темпе срабатывания таймера.
    digitalWrite( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
}
