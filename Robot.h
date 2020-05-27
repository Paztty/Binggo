#ifndef ROBOT_H
#define ROBOT_H
// library for feel enviroment 
#include "Arduino.h"
#include "HCSR04.h"

// library for caculator
#include <math.h>

/////////Define-------------------------------------------------------------------------
#define wheel_step_per_pulse      10 //distander with one pulse feedback by mm
#define width_from_two_wheel       245 // distander beetween two wheel by mm
#define PI   3.14159265358979323846 /* pi */
#define perimeter_robot           width_from_two_wheel*PI

class Robot
{
    public:
    // public var
        unsigned int L_FB_Counter, R_FB_Counter;    
        int MT_L_1, MT_L_2, MT_L_FB, MT_R_1, MT_R_2, MT_R_FB ; // l298D input control moto
        int MT_L_EN, MT_R_EN;

    // public func
        void init();
        void Said( String Words);
        uint8_t Run_caculator ( int angle, int distance_from_here);


     private:
        // unsigned int L_FB_Counter, R_FB_Counter;    
        // int MT_L_1, MT_L_2, MT_L_FB, MT_R_1, MT_R_2, MT_R_FB ; // l298D input control moto
        // int MT_L_EN, MT_R_EN;
};

#endif
