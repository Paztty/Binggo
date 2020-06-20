#include "Robot.h"
#include "HCSR04.h"


///////////////////////////////////////////////var///////////////////////////////////////////////////
//  // Initialize sensor that uses digital pins 13 and 12.
//        distanceSensor.measureDistanceCm());    // read distance function
UltraSonicDistanceSensor distanceSensor(12, 13);

void Robot::init()
{
    Serial.begin(115200);

    head.attach(9);
    MT_L_FB = 3;
    MT_R_FB = 2;
    pinMode(MT_R_FB , INPUT_PULLUP); // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
    pinMode(MT_L_FB , INPUT_PULLUP); // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
    
    MT_L_1 = 5;
    MT_L_2 = 7;
    pinMode(MT_L_1 , OUTPUT); 
    pinMode(MT_L_2 , OUTPUT); 
    MT_R_1 = 6; 
    MT_R_2 = 8; 
    pinMode(MT_R_1 , OUTPUT); 
    pinMode(MT_R_2 , OUTPUT); 
    //MT_L_EN = 9;
    //MT_R_EN = 10;
    head.write(90);
    Step_run_fl = 1;
    delay(100);
}

// main function
void Robot::Run()
{
    double distance;
        switch (Get_free_land())
        {
        case front:
        {
            Said("Front");
            R_FB_Counter = 1;
            L_FB_Counter = 1;
            L_FB_Setpoit = 1000;
            R_FB_Setpoit = 1000;
            while (Step_run_fl == 1 )
            {
            distance = distanceSensor.measureDistanceCm();
            //Serial.println(distance);
            Serial.print(R_FB_Counter);
            Serial.print("     --     ");
            Serial.println(L_FB_Counter);
            if(distance < free_distance && distance > 0)
            {
                Said("On distance break");
                Serial.println(distance);
                Step_run_fl = 0;
                Stop(Step_run_fl);
            }                
            Run_by_pulse(1,L_PWM,1,R_PWM);
            delay(100);
            }
            break;
        }

        case left:
        {
            Said("left");
            R_FB_Counter = 0;
            L_FB_Counter = 0;
            turnBack();
            head.write(90);
            turnLeft();
            break;
        }

        case right:
        { 
            Said("Right");
            R_FB_Counter = 0;
            L_FB_Counter = 0;
            turnBack();
            head.write(90);
            turnRight();
            break;
        }

        case behind:
        {
            Said("behind");
            R_FB_Counter = 1;
            L_FB_Counter = 1;
            L_FB_Setpoit = 30;
            R_FB_Setpoit = 30;
            turnBack();
            while (Step_run_fl == 1 )
            {
            Run_by_pulse(0,L_PWM,1,R_PWM);
            }
            head.write(90);
            break;
        }
    }
    
}


void Robot::Said( String Works)
{
    Serial.println(Works);
}

void Robot::Run_caculator(int angle, int distance_from_here) // bán kính cung tròn xe quay = độ rộng xe *số xung nhỏ / (số xung nhỏ + số xung lớn)
{
    float real_angle = angle*PI/180;
    float A = distance_from_here/2 * tan(real_angle);
    float B = distance_from_here/2 / cos(real_angle);

    float r = distance_from_here/2/A * B;
    this->small_circle = ((r-width_from_two_wheel/2)*PI*real_angle)/90;
    this->big_circle = (small_circle*(r + width_from_two_wheel/2))/(r-width_from_two_wheel/2);

    // float small_circle = ((distance_from_here - width_from_two_wheel/2)*PI*angle)/90;
    // float big_circle = ((distance_from_here + width_from_two_wheel/2)*PI*angle)/90;
    

    // show equart on terminal
    String show_caculator = String(this->small_circle);

    Serial.print("Small_circle: ");
    Serial.println(show_caculator);

    show_caculator = String(this->big_circle);

    Serial.print("Big_circle: ");
    Serial.println(show_caculator);

    show_caculator = String(r);
    
    Serial.print("r: ");
    Serial.println(show_caculator);

    // show_caculator = String(width_from_two_wheel);
    // Serial.print("r: ");
    // Serial.println(show_caculator);
}

void Robot::Run_by_pulse(uint8_t Dil_L, uint8_t PWM_L, uint8_t Dil_R, uint8_t PWM_R)
{
    if(Step_run_fl == 1){
    if ( Dil_L == 1 ){
         analogWrite(MT_L_1, 255 - PWM_L);
         digitalWrite(MT_L_2, HIGH);
    }
    else if ( Dil_L == 0 ) {
         analogWrite(MT_L_1, PWM_L);
         digitalWrite(MT_L_2, LOW);
    }
    if ( Dil_R == 1 ){
         analogWrite(MT_R_1, 255 - PWM_R);
         digitalWrite(MT_R_2, HIGH);
    }
    else if ( Dil_R == 0 ) {
         analogWrite(MT_R_1, PWM_R);
         digitalWrite(MT_R_2, LOW);
    }
    // clear Step_run_fl when run done a step.
        if(R_FB_Counter >= R_FB_Setpoit && L_FB_Counter >= L_FB_Setpoit){
            Step_run_fl = 0;
            Stop(Step_run_fl);
            Serial.print(R_FB_Counter);
            Serial.print("     --     ");
            Serial.println(L_FB_Counter);
            R_FB_Counter = 0;
            L_FB_Counter = 0;
            Said("Crash in run by pulse");
        }
    } 
    delay(50);  
}

void Robot::Stop(uint8_t flag){
     if(flag == 0){
       analogWrite(MT_L_1, 0);
       digitalWrite(MT_L_2, LOW);
       analogWrite(MT_R_1, 0);
       digitalWrite(MT_R_2, LOW);
    } 
}


uint8_t Robot::Get_free_land()
    {
        uint8_t free_land = behind;
        double distance = distanceSensor.measureDistanceCm();
            if(distanceSensor.measureDistanceCm() > free_distance)
            {
              free_land = front; 
              goto out; 
            }

            head.write(0);
            delay(500);
            if(distanceSensor.measureDistanceCm() > free_distance)
            {
              free_land = left; 
              goto out; 
            }
                
            head.write(180);
            delay(500);
            if(distanceSensor.measureDistanceCm() > free_distance)
            {
              free_land = right; 
              goto out; 
            }

            out:
            Serial.println(free_land); 
            Step_run_fl = 1;          
            return free_land;
    }  
void Robot::turnBack()
{
    analogWrite(MT_L_1, L_PWM-20);
    digitalWrite(MT_L_2, LOW);

    analogWrite(MT_R_1, R_PWM-20);
    digitalWrite(MT_R_2, LOW);
  
    while(Step_run_fl == 1){
            Serial.print(R_FB_Counter);
            Serial.print("     --     ");
            Serial.println(L_FB_Counter); 
    // clear Step_run_fl when run done a step.
        if(R_FB_Counter > turnBackCounter && L_FB_Counter > turnBackCounter){
            Step_run_fl = 0;
            Stop(Step_run_fl);
            Serial.print(R_FB_Counter);
            Serial.print("     --     ");
            Serial.println(L_FB_Counter);
            R_FB_Counter = 0;
            L_FB_Counter = 0;
            Said("Crash in run by pulse");
        } 
      } 
    Step_run_fl = 1;
    delay(100);
}

void Robot::turnLeft()
{
    analogWrite(MT_L_1, 0);
    digitalWrite(MT_L_2, LOW);

    analogWrite(MT_R_1,255 -  R_PWM);
    digitalWrite(MT_R_2, HIGH);
  
    while(Step_run_fl == 1){
            Serial.print(R_FB_Counter);
            Serial.print("     --     ");
            Serial.println(L_FB_Counter); 
    // clear Step_run_fl when run done a step.
        if(R_FB_Counter > turnCounter ){
            Step_run_fl = 0;
            Stop(Step_run_fl);
            Serial.print(R_FB_Counter);
            Serial.print("     --     ");
            Serial.println(L_FB_Counter);
            R_FB_Counter = 0;
            L_FB_Counter = 0;
            Said("Crash in run by pulse");
        } 
      } 
    Step_run_fl = 1;
}

void Robot::turnRight()
{
    analogWrite(MT_L_1,255 -  L_PWM);
    digitalWrite(MT_L_2, HIGH);

    analogWrite(MT_R_1, 0);
    digitalWrite(MT_R_2, LOW);
  
    while(Step_run_fl == 1){
            Serial.print(R_FB_Counter);
            Serial.print("     --     ");
            Serial.println(L_FB_Counter); 
    // clear Step_run_fl when run done a step.
        if(L_FB_Counter > turnCounter){
            Step_run_fl = 0;
            Stop(Step_run_fl);
            Serial.print(R_FB_Counter);
            Serial.print("     --     ");
            Serial.println(L_FB_Counter);
            R_FB_Counter = 0;
            L_FB_Counter = 0;
            Said("Crash in run by pulse");
        } 
      } 
    Step_run_fl = 1;
}


void Robot::Timer_for_sevice()
{
    Run_sevice++;
    Stop_sevice++;
}
