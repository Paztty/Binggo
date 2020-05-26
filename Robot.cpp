#include "Robot.h"



///////////////////////////////////////////////var///////////////////////////////////////////////////
UltraSonicDistanceSensor distanceSensor(13, 12);  // Initialize sensor that uses digital pins 13 and 12.
//        distanceSensor.measureDistanceCm());    // read distance function

void Robot::Said( String Works)
{
    Serial.println(Works);
}