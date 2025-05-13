<<<<<<< HEAD
#include "cmsis_os.h"                               // CMSIS RTOS header file

extern void thread_sample (void const *argument);   // prototype

typedef struct  a {
  char y[100];
} a_element;

osThreadDef   (thread_sample, osPriorityBelowNormal, 2, 100);

osPoolDef(MyPool, 10, struct a);
osMessageQDef(MyMessage, 10, a_element *);
osMailQDef(MyMail, 10, a_element);


=======
#include "cmsis_os.h"                               // CMSIS RTOS header file

extern void thread_sample (void const *argument);   // prototype

typedef struct  a {
  char y[100];
} a_element;

osThreadDef   (thread_sample, osPriorityBelowNormal, 2, 100);

osPoolDef(MyPool, 10, struct a);
osMessageQDef(MyMessage, 10, a_element *);
osMailQDef(MyMail, 10, a_element);


>>>>>>> 5881ee9d9a49cdc272890e0007b9baca97e186f3
