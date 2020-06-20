#ifndef ROBOT_H
#define ROBOT_H
// library for feel enviroment 
#include "Arduino.h"
#include "HCSR04.h"
#include <Servo.h>
// library for caculator
#include <math.h>

/////////Define-------------------------------------------------------------------------
#define wheel_step_per_pulse      1 //distander with one pulse feedback by mm
#define width_from_two_wheel       245 // distander beetween two wheel by mm
#define PI   3.14159265358979323846 /* pi */
#define perimeter_robot           width_from_two_wheel*PI


//Sevice interval
#define Stop_interval 1000
#define Run_interval  100
// Defien direction
#define left 1
#define right 3
#define front 2
#define behind 0
#define free_distance 25
#define turnCounter 60
#define turnBackCounter 30
class Robot
{
    public:
    // public var
        uintmax_t Run_sevice, Stop_sevice;
        unsigned int L_FB_Counter, R_FB_Counter, L_FB_Setpoit, R_FB_Setpoit;    
        uint8_t MT_L_1, MT_L_2, MT_L_FB, MT_R_1, MT_R_2, MT_R_FB ; // l298D input control moto
        uint8_t Step_run_fl;
        uint8_t L_PWM = 113, R_PWM = 106;
        //int MT_L_EN, MT_R_EN;
        Servo head;
        float small_circle, big_circle;
    // public func
        void init();

        void Run();

        void Run_calib(uint8_t L_PWM, uint8_t R_PWM);
        void Said(String Words);
        void Run_caculator ( int angle, int distance_from_here);
        void Run_by_pulse(uint8_t Dil_L, uint8_t PWM_L, uint8_t Dil_R, uint8_t PWM_R);
        void Stop(uint8_t flag);
        uint8_t Get_free_land();
        void Timer_for_sevice();
        void turnBack();
        void turnLeft();
        void turnRight();
     private:
        // unsigned int L_FB_Counter, R_FB_Counter;    
        // int MT_L_1, MT_L_2, MT_L_FB, MT_R_1, MT_R_2, MT_R_FB ; // l298D input control moto
        // int MT_L_EN, MT_R_EN;
};

#endif
