// Code này rất cute cho một con robot cute không kém
// include librarry
// Tao vừa up lên git đấy
#include <avr/interrupt.h>

#include "Robot.h"
#include "HCSR04.h"


// Note for setup timer
          // BAUDRATE 115200
          // TCNT1 (start_counter) = 65536 - (timer * (16 * 10^6)/prescaler)

          //prescaler 1         CS12: 0   CS11: 0   CS10: 1
          //prescaler 8         CS12: 0   CS11: 1   CS10: 0
          //prescaler 256       CS12: 1   CS11: 0   CS10: 0
          //prescaler 1024      CS12: 1   CS11: 0   CS10: 1

          /*prescaler 64        CS12: 0   CS11: 1   CS10: 1
            0.100 s -> TCNT1 = 65536 - (0.100 * (16 * 10^6)/64) = 40536      100.1 ms
            0.095 s -> TCNT1 = 65536 - (0.095 * (16 * 10^6)/64) = 41786      95.01 ms
            0.090 s -> TCNT1 = 65536 - (0.090 * (16 * 10^6)/64) = 43036      90.09 ms
            0.085 s -> TCNT1 = 65536 - (0.085 * (16 * 10^6)/64) = 44286      85.09 ms
            0.080 s -> TCNT1 = 65536 - (0.080 * (16 * 10^6)/64) = 45536      80.08 ms
            0.075 s -> TCNT1 = 65536 - (0.075 * (16 * 10^6)/64) = 46786      75.08 ms
            0.070 s -> TCNT1 = 65536 - (0.070 * (16 * 10^6)/64) = 48036      70.07 ms
            0.065 s -> TCNT1 = 65536 - (0.065 * (16 * 10^6)/64) = 49286      65.07 ms
            0.060 s -> TCNT1 = 65536 - (0.060 * (16 * 10^6)/64) = 50536      60.06 ms
            0.055 s -> TCNT1 = 65536 - (0.055 * (16 * 10^6)/64) = 51786      55.06 ms
            0.050 s -> TCNT1 = 65536 - (0.050 * (16 * 10^6)/64) = 53036      50.05 ms
            0.045 s -> TCNT1 = 65536 - (0.045 * (16 * 10^6)/64) = 54286      45.05 ms
            0.040 s -> TCNT1 = 65536 - (0.040 * (16 * 10^6)/64) = 55536      40.04 ms
            0.035 s -> TCNT1 = 65536 - (0.035 * (16 * 10^6)/64) = 56786      35.04 ms
            0.030 s -> TCNT1 = 65536 - (0.030 * (16 * 10^6)/64) = 58036      30.03 ms
            0.025 s -> TCNT1 = 65536 - (0.025 * (16 * 10^6)/64) = 59286      25.03 ms
            0.020 s -> TCNT1 = 65536 - (0.020 * (16 * 10^6)/64) = 60536      20.02 ms
            0.015 s -> TCNT1 = 65536 - (0.015 * (16 * 10^6)/64) = 61786      15.02 ms
            0.010 s -> TCNT1 = 65536 - (0.010 * (16 * 10^6)/64) = 63036      10.01 ms
            0.005 s -> TCNT1 = 65536 - (0.005 * (16 * 10^6)/64) = 64286      5.005 ms
            0.001 s -> TCNT1 = 65536 - (0.001 * (16 * 10^6)/64) = 65286      1.021 ms
          */


//User define
///Define parametter for timer
#define BAUDRATE        							115200
#define PRESCALER       							64
#define TIMER           							20              //unit: milliseconds
#define START_COUNTER   							(65536 - (TIMER * 16 *(unsigned long)1000/PRESCALER))

#define NUMBER_OF_SERVICE             5
///Define.........................
#define Said_interval                 10000




//Global var
    ///vitrual timer
    unsigned int Service_timer[NUMBER_OF_SERVICE];

    ///// Distance
    // Initialize sensor that uses digital pins 13 and 12.
    ///// Servo
    Servo head;
    ///// Robot
    Robot Binggo;
    //buffer for debug
    String binggo_number_define;

    //fl var
    uint8_t ss1_fl, ss2_fl, pos;

// Main function here..........................................................................................

void setup() {
// setup Serial
// Servo setup
//head.attach(9);
// Robot setup
Binggo.init();
// setup timer for calendar
  cli();																// tắt ngắt toàn cục

  /* Reset Timer/Counter1 */
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = 0;

  /* Setup Timer/Counter1 */
  TCCR2B |= (0 << CS22) | (1 << CS21) | (1 << CS20);  // prescale = 64
  TCNT2 = START_COUNTER;
  TIMSK2 = (1 << TOIE2); 							 // overflow interrupt enable
  sei(); 															 // cho phép ngắt toàn cục
  
// setup IO
 
// Hello to bedug windows
  Binggo.Said("Helloooo");
  attachInterrupt(1, FB_R_Counter, FALLING);
  attachInterrupt(0, FB_L_Counter, FALLING);


//Test............cant delete when done
}

void loop() {
  Binggo.Run();
}


// End of main................................................................................................

//Counter feedback wheel
void FB_L_Counter()
{
    Binggo.L_FB_Counter++;
}

void FB_R_Counter()
{
    Binggo.R_FB_Counter++;
}




// Interrupt for vitrual timer
ISR (TIMER2_OVF_vect)
{
  Binggo.Timer_for_sevice();
  for (unsigned char i = 0; i < NUMBER_OF_SERVICE; i++)
  {
    Service_timer[i]++;
  }  
  TCNT2 = START_COUNTER;
}
