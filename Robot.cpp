#include "Robot.h"



///////////////////////////////////////////////var///////////////////////////////////////////////////
UltraSonicDistanceSensor distanceSensor(13, 12);  // Initialize sensor that uses digital pins 13 and 12.
//        distanceSensor.measureDistanceCm());    // read distance function


void Robot::init()
{
    MT_L_FB = 2;
    MT_R_FB = 3;
    pinMode(MT_R_FB , INPUT_PULLUP); // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
    pinMode(MT_L_FB , INPUT_PULLUP); // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
}




void Robot::Said( String Works)
{
    Serial.println(Works);
}

uint8_t Robot::Run_caculator(int angle, int distance_from_here) // bán kính cung tròn xe quay = độ rộng xe *số xung nhỏ / (số xung nhỏ + số xung lớn)
{
    float real_angle = angle*PI/180;
    float A = distance_from_here/2 * tan(real_angle);
    float B = distance_from_here/2/cos(real_angle);

    float r = distance_from_here/2/A * B;
    float small_circle = ((r-width_from_two_wheel/2)*PI*real_angle)/90;
    float big_circle = (small_circle*(r + width_from_two_wheel/2))/(r-width_from_two_wheel/2);


    // float small_circle = ((distance_from_here - width_from_two_wheel/2)*PI*angle)/90;
    // float big_circle = ((distance_from_here + width_from_two_wheel/2)*PI*angle)/90;


    // show equart on terminal
    String show_caculator = String(small_circle);

    Serial.print("A: ");
    Serial.println(show_caculator);

    show_caculator = String(big_circle);

    Serial.print("B: ");
    Serial.println(show_caculator);

    show_caculator = String(r);
    
    Serial.print("r: ");
    Serial.println(show_caculator);

    // show_caculator = String(width_from_two_wheel);
    // Serial.print("r: ");
    // Serial.println(show_caculator);
}