<<<<<<< HEAD

#include "cmsis_os.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void Thread (void const *argument);                             // thread function
osThreadId tid_Thread;                                          // thread id
osThreadDef (Thread, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread (void) {

  tid_Thread = osThreadCreate (osThread(Thread), NULL);
  if (!tid_Thread) return(-1);
  
  return(0);
}

void Thread (void const *argument) {

  while (1) {
    ; // Insert thread code here...
    osThreadYield ();                                           // suspend thread
  }
}
=======

#include "cmsis_os.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void Thread (void const *argument);                             // thread function
osThreadId tid_Thread;                                          // thread id
osThreadDef (Thread, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread (void) {

  tid_Thread = osThreadCreate (osThread(Thread), NULL);
  if (!tid_Thread) return(-1);
  
  return(0);
}

void Thread (void const *argument) {

  while (1) {
    ; // Insert thread code here...
    osThreadYield ();                                           // suspend thread
  }
}
>>>>>>> 5881ee9d9a49cdc272890e0007b9baca97e186f3
