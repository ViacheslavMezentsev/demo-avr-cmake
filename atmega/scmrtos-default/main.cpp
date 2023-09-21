#include "main.h"

tick_count_t mseconds = 0;
uint32_t seconds = 0;

#if scmRTOS_SYSTIMER_HOOK_ENABLE

/**
 * @brief   Пользовательский обработчик системного таймера.
 * 
 */
void OS::system_timer_user_hook()
{
#if scmRTOS_SYSTIMER_NEST_INTS_ENABLE
    TCritSect cs;
#endif
    mseconds++;

    if ( mseconds == 1000 )
    {
        mseconds = 0;
        seconds++;
    }
}

#endif


/**
 * @brief   Точка входа в основную программу.
 * 
 */
int main()
{
    // Запрещаем прерывания от системного таймера.
    OS::lock_system_timer();

    // Настройка последовательного порта USART.
    USART_Init();

    // Настраиваем таймер для scmRTOS.
    TIMER_CS_REG = ( 1 << WGM12 ) | ( 1 << CS11 ) | ( 1 << CS10 );

    OCR1AL = 250;

    // Разрешаем прерывания от системного таймера.
    OS::unlock_system_timer();

    // Запуск scmRTOS.
    OS::run();
}
