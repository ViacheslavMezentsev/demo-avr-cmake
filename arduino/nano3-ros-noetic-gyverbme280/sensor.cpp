#include <ros.h>
#include <std_msgs/Float32.h>
#include <GyverBME280.h>

bool available = false;
long publisher_timer = 0;
GyverBME280 bme;
std_msgs::Float32 temp_msg, hum_msg, pres_msg;
ros::Publisher pub_temp( "arduino/temperature", &temp_msg );
ros::Publisher pub_hum( "arduino/humidity", &hum_msg );
ros::Publisher pub_pres( "arduino/pressure", &pres_msg );
ros::NodeHandle nh;

void setup()
{
    // Настраиваем вывод светодиода.
    pinMode( LED_BUILTIN, OUTPUT );

    // Проверка доступности датчика BMP280 (0x76) на частоте 400 кГц.
    available = bme.begin( 0x76, 400'000 );

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
        temp_msg.data = bme.readTemperature();
        pub_temp.publish( &temp_msg );

        // Влажность.
        hum_msg.data = bme.readHumidity();
        pub_hum.publish( &hum_msg );

        // Давление.
        pres_msg.data = bme.readPressure();
        pub_pres.publish( &pres_msg );
    }

    // Настраиваем время срабатывания таймера.
    publisher_timer = millis() + 1000;

    nh.spinOnce();

    // Переключаем светодиод в темпе срабатывания таймера.
    digitalWrite( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
}
