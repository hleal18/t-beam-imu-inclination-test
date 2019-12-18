#include <Arduino.h>
#include "IMUSensor/IMUSensor.h"

IMUSensor imu(SDA, SCL, scale_16g, scale_500dps, 0.06);
float angle_pitch = 0;
float angle_roll = 0;

double angle_pitch_acc;
double angle_roll_acc;

double angle_pitch_output;
double angle_roll_output;

long acc_total_vector;
bool set_gyro_angles = false;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    imu.begin();
}

void loop()
{
    // put your main code here, to run repeatedly:
    Axis<int16_t> rotation_rate = imu.get_gyroscope_readings();
    Axis<int16_t> accel = imu.get_accelerometer_readings();
    
    Axis<double> acceleration = imu.get_acceleration();
    //Serial.println(acceleration.z);
    
    
    //Serial.println(rotation_rate.x);
    // (1 / 250)/ 131
    // 131 from datasheet
    // 250hz from frequency of sensor readings. (4ms period)
    // angle_pitch += rotation_rate.x * 0.0000305;
    // angle_roll += rotation_rate.y * 0.0000305;
    angle_pitch += rotation_rate.x * 0.0000611;
    angle_roll += rotation_rate.y * 0.0000611;
    
    //Serial.print("pitch, x: "); Serial.print(angle_pitch); Serial.print(" roll, y: "); Serial.println(angle_roll);
    
    angle_pitch += angle_roll * sin(rotation_rate.z * 0.000001066);
    angle_roll -= angle_pitch * sin(rotation_rate.z * 0.000001066);
    
    Serial.print("pitch, x: "); Serial.print(angle_pitch); Serial.print(" roll, y: "); Serial.println(angle_roll);
    
    acc_total_vector = sqrt((acceleration.x*acceleration.x)+(acceleration.y*acceleration.y)+(acceleration.z*acceleration.z));
    
    angle_pitch_acc = (asin(acceleration.y / acc_total_vector) ? asin(acceleration.y/acc_total_vector) : 90.0)*57.296;
    angle_roll_acc = asin(acceleration.x/acc_total_vector)* -57.296;
    
    //Serial.print("pitch, x: "); Serial.print(angle_pitch_acc); Serial.print(" roll, y: "); Serial.println(angle_roll_acc);
    
    // if(set_gyro_angles) {
    //     angle_pitch = angle_pitch *0.9996 + angle_pitch_acc * 0.0004;
    //     angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;
    // }
    // else {
    //     angle_pitch = angle_pitch_acc;
    //     angle_roll = angle_roll_acc;
    //     set_gyro_angles = true;
    // }
    
    // //Serial.print("pitch, x: "); Serial.print(angle_pitch); Serial.print(" roll, y: "); Serial.println(angle_roll);
    
    // angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;
    // angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;
    
    // //Serial.print("pitch, x: "); Serial.print(angle_pitch_output); Serial.print(" roll, y: "); Serial.println(angle_roll_output);
    
    delay(4);
}