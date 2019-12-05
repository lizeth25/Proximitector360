#define TRIG 0
#define ECHO 1
#define SECTRIG 6
#define SECECHO 12
#define OVERSMPL 5
#define MOTOR1 3
#define MOTOR2 10
 
float waveTime, waveTime2, distance, distance2, curr_dist, curr_dist2;

void setup();
void loop();
float readSensor();
int sampling();
//void dist_motor1(float power);
void dist_motor1(int power);


float readSensor2();
int sampling2();
//void dist_motor2(float power);
void dist_motor2(int power);

void setup() {
  Serial.begin (9600);

  //Setting our trigs to output and echos to input
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(SECTRIG, OUTPUT);
  pinMode(SECECHO, INPUT);

  //Setting our motors to outputs and turning them off
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR1, LOW);

  //pinMode(MOTOR2, OUTPUT);
  //pinMode(MOTOR2, LOW);

 
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

  //calculating the average distance to display from 5 samplings for sensor 1
  
  int avg_dist = sampling();
  Serial.print(avg_dist);
  Serial.println(" mode motor 1");
  dist_motor1(avg_dist);
  //calculating the average distance to display from 5 samplings for sensor 2
  int avg_dist_m2 = sampling2();
  Serial.print(avg_dist_m2);
  Serial.println(" mode motor 2");
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
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Reading the Echo Pin
  waveTime = pulseIn(ECHO, HIGH);
  // Speed of sound is 343 m/s, use to determine distance from our waveTime
  distance = (waveTime / 2.0) * 0.0343;
  return distance;
}

float readSensor2(){
  // Write to the Trigger Pin to begin sending wave
  digitalWrite(SECTRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(SECTRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SECTRIG, LOW);

  // Reading the Echo Pin
  waveTime2 = pulseIn(SECECHO, HIGH);
  // Speed of sound is 343 m/s, use to determine distance from our waveTime
  distance2 = (waveTime2 / 2.0) * 0.0343;
  return distance2;
}

/*This function calculates the average of five reads and returns it as
 * a value from 1-4 depending on the distance of an object. Closer 
 * objects are near 1 and farther objects will be closer to 4. This is 
 * for the first sensor
 */
int sampling(){
  float reads = 0;
  for (int i = 0; i < OVERSMPL; i++){
    curr_dist = readSensor(); //calls the read function once
    /*
    Serial.print("Curr: ");
    Serial.println(curr_dist);
    */
    //if our distance is in range add to our reads in order to average
    if (curr_dist <= 400.0 && curr_dist >= 2.0) {
      reads += curr_dist;
    }
    if (curr_dist > 400.0 || curr_dist < 2.0){
      reads += 400;
    }
  }

  reads = reads/OVERSMPL;
  Serial.print("Avg Sensor 1: ");
  Serial.println(reads);

  //Returning the case in which our average distance belongs in 
  if (reads > 300 && reads <= 400 || reads>400 || reads<2){
    return 6;
  }
  else if (reads > 200 && reads <= 300){
    return 5;
  }
  else if (reads > 150 && reads <= 200){
    return 4;
  }
  else if (reads > 100 && reads <= 150){
    return 3;
  }
  else if (reads > 50 && reads <= 100){
    return 2;
  }
  else{
    return 1;
  }
}

/*This function calculates the average of five reads and returns it as
 * a value from 1-4 depending on the distance of an object. Closer 
 * objects are near 1 and farther objects will be closer to 4. This is
 * for the second sensor
 */
int sampling2(){
  float reads = 0;
  for (int i = 0; i < OVERSMPL; i++){
    curr_dist2 = readSensor2(); //calls the read function once
    /*
    Serial.print("Curr: ");
    Serial.println(curr_dist2);*/
    
    //if our distance is in range add to our reads in order to average
    if (curr_dist2 <= 400.0 && curr_dist2 >= 2.0) {
      reads += curr_dist2;
    }
    if (curr_dist > 400.0 || curr_dist < 2.0){
      reads += 400;
    }
  }

  reads = reads/OVERSMPL;
  Serial.print("Avg Sensor 2: ");
  Serial.println(reads);

  //Returning the case in which our average distance belongs in 
  if (reads > 300 && reads <= 400 || reads>400 || reads<2){
    return 6;
  }
  else if (reads > 200 && reads <= 300){
    return 5;
  }
  else if (reads > 150 && reads <= 200){
    return 4;
  }
  else if (reads > 100 && reads <= 150){
    return 3;
  }
  else if (reads > 50 && reads <= 100){
    return 2;
  }
  else{
    return 1;
  }
}


void dist_motor1(int power){
  int avg_dis = power*10000;
  
  // Set PORTD0 for 1 ms
  PORTD |= (1 << PORTD0);
  delayMicroseconds(avg_dis);
  //delayMicroseconds(600000.0 - avg_dis);
  
  // Clear PORTD0
  PORTD &= ~(1 << PORTD0);
  //delayMicroseconds(avg_dis);
  delayMicroseconds(600000.0 - avg_dis);
  
}


void dist_motor2(int power){
  int avg_dis = power*10000;
  
  // Set PORTB6 for 1 ms
  PORTB |= (1 << PORTB6);
  delayMicroseconds(avg_dis);
  //delayMicroseconds(600000.0 - avg_dis);
  
  // Clear PORTB6
  PORTB &= ~(1 << PORTB6);
  //delayMicroseconds(avg_dis);
  delayMicroseconds(600000.0 - avg_dis);
  
}
