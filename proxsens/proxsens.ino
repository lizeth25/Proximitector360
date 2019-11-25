#include <Arduino.h>
#include <stdint.h>
#include <math.h>

#define TRIG 0
#define ECHO 1
#define SECTRIG 6
#define SECECHO 12
#define OVERSMPL 5
 
float waveTime, distance, curr_dist;

void setup();
void loop();
float readSensor();
float sampling();
void dist_motor1(float power);
void vel_motor1(float power);

float readSensor2();
float sampling2();
void dist_motor2(float power);

void setup() {
  Serial.begin (9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(SECTRIG, OUTPUT);
  pinMode(SECECHO, INPUT);
  //pinMode(LED, OUTPUT); //currently testing with LED, will replace with haptic motor
  //pinMode(LED,LOW); //currently testing with LED, will replace with haptic motor
  
  // Set bit 0 in port D to output
  DDRD |= (1 << PORTD0);
  // Clear PORTD0 in setup
  PORTD &= ~(1 << PORTD0);

  // Set bit 0 in port D to output
  DDRB |= (1 << PORTB6);
  // Clear PORTD0 in setup
  PORTB &= ~(1 << PORTB6);

}
 
void loop() { 

  //calculating the average distance to display from 5 samplings
  //float avg_dist = 250.15;
  
  float avg_dist = sampling();
  Serial.print(avg_dist);
  Serial.println(" cm for motor 1");
  dist_motor1(avg_dist);

  float avg_dist_m2 = sampling2();
  Serial.print(avg_dist_m2);
  Serial.println(" cm for motor 2");
  dist_motor2(avg_dist_m2);

}

/*
 * Function readSensor will return the distance of one read using the 
 * HC-SR04 proximity sensor. In this function the trig pin will 
 * be turned on and then off to send a wave an ultrasonic sound 
 * wave out into the environment, and then the echo pin will be 
 * used to read that wave's travelling time. We can use the 
 * distance formula : distance = speed * time to calculate the 
 * distance of an object.
 */

float readSensor(){
  // Write to the Trigger Pin to begin sending wave
  digitalWrite(TRIG, LOW);
  //digitalWrite(TRIG2,LOW)
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  delay(2); // A delay of 60ms to allow wave to travel

  // Reading the Echo Pin
  waveTime = pulseIn(ECHO, HIGH);
  // Speed of sound is 343 m/s, use to determine distance from our waveTime
  distance = (waveTime / 2.0) * 0.0343;
  return distance;
}

float readSensor2(){
  // Write to the Trigger Pin to begin sending wave
  digitalWrite(SECTRIG, LOW);
  //digitalWrite(TRIG2,LOW)
  delayMicroseconds(2);
  digitalWrite(SECTRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SECTRIG, LOW);
  delay(2); // A delay of 60ms to allow wave to travel

  // Reading the Echo Pin
  waveTime = pulseIn(SECECHO, HIGH);
  // Speed of sound is 343 m/s, use to determine distance from our waveTime
  distance = (waveTime / 2.0) * 0.0343;
  return distance;
}

//This function calculates the average of five reads and returns it
float sampling(){
  float reads = 0;
  for (int i = 0; i < OVERSMPL; i++){
    curr_dist = readSensor(); //calls the read function once

    //if our distance is in range add to our reads in order to average
    if (curr_dist <= 400.0 && curr_dist >= 2.0) {
      reads += curr_dist;
    }
  }
  return reads/OVERSMPL;
}

//This function calculates the average of five reads and returns it
float sampling2(){
  float reads = 0;
  for (int i = 0; i < OVERSMPL; i++){
    curr_dist = readSensor2(); //calls the read function once

    //if our distance is in range add to our reads in order to average
    if (curr_dist <= 400.0 && curr_dist >= 2.0) {
      reads += curr_dist;
    }
  }
  return reads/OVERSMPL;
}

void dist_motor1(float power){
  float avg_dis = power*100;
  
  // Set PORTD0 for 1 ms
  PORTD |= (1 << PORTD0);
  delayMicroseconds(40000.0-avg_dis);
  // Clear PORTD0
  PORTD &= ~(1 << PORTD0);
  delayMicroseconds(avg_dis);
  
}

void dist_motor2(float power){
  float avg_dis = power*100;
  
  // Set PORTD0 for 1 ms
  PORTB |= (1 << PORTB6);
  delayMicroseconds(40000.0-avg_dis);
  // Clear PORTD0
  PORTB &= ~(1 << PORTB6);
  delayMicroseconds(avg_dis);
  
}


void vel_motor1(float power){
  float avg_dist = power*10;

  
  // Set PORTD0 for 1 ms
  PORTD |= (1 << PORTD0);
  delayMicroseconds(avg_dist);
  // Clear PORTD0
  PORTD &= ~(1 << PORTD0);
  delayMicroseconds(4000.0-avg_dist);
}
