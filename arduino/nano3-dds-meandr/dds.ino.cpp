#include "PinChangeInterrupt.h"
#include "sinwave.h"

uint16_t PhaseShift { 12 };     ///< код частоты

/**
 * @brief   Обработчик прерывания от АЦП.
 * 
 */
ISR( ADC_vect )
{
    // Считываем старший байт значения АЦП.
    PhaseShift = ( ADCH << 4U ) + 1U;
}


/**
 * @brief   Настройка АЦП.
 * 
 */
void ADC_init()
{
    // Сбрасываем регистр ADCSRB.
    ADCSRB = 0;

    // Опорное напряжение: источник питания мк.
    bitClear( ADMUX, REFS1 );
    bitSet( ADMUX, REFS0 );

    // Формат результата: 8 бит ADCH + 2 бита ADCL
    bitSet( ADMUX, ADLAR );

    // Выбираем КАНАЛ АЦП = AD0
    bitClear( ADMUX, MUX3 );
    bitClear( ADMUX, MUX2 );
    bitClear( ADMUX, MUX1 );
    bitClear( ADMUX, MUX0 );

    // Режим АЦП ВКЛЮЧЕН
    bitSet( ADCSRA, ADEN );

    // Автозапуск ВКЛЮЧЕН
    bitSet( ADCSRA, ADATE );

    // Запрещаем прерывания АЦП.
    bitClear( ADCSRA, ADIE );

    // Предделитель на 128.
    bitSet( ADCSRA, ADPS2 );
    bitSet( ADCSRA, ADPS1 );
    bitSet( ADCSRA, ADPS0 );

    // Преобразование остановлено.
    bitClear( ADCSRA, ADSC );
}


/**
 * @brief   Обработчик прерывания по нажатию на кнопку.
 * 
 */
void OnButtonPressed()
{
    static bool active { false };

    bitClear( ADCSRA, ADIE );

    active = !active;

    digitalWrite( LED_BUILTIN, active ? HIGH : LOW );

    if ( active )
    {
        bitSet( ADCSRA, ADIE );
        bitSet( ADCSRA, ADSC );
    }
}


/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    // Настраиваем АЦП.
    ADC_init();

    // Порт D (ЦАП DDS) на выход.
    DDRD = 0b1111'1111;

    const uint8_t pin {8};

    // Аккумулятор фазы.
    uint16_t PhaseAccum {0};

    // Кнопка режима работы: ЗАПУСК/ОСТАНОВ.
    pinMode( pin, INPUT_PULLUP );

    // Встроенный светодиод.
    pinMode( LED_BUILTIN, OUTPUT );
   
    // Подключаем прерывания для кнопки.
    attachPCINT( digitalPinToPCINT( pin ), OnButtonPressed, FALLING );

    // Формируем сигнал.
    while ( true ) 
    {
        PORTD = sinewave[ PhaseAccum ];

        PhaseAccum += PhaseShift;
    }
}
