#define TRIG 0
#define ECHO 1
#define LED 2
#define OVERSMPL 5
 
float waveTime, distance, curr_dist;

void setup();
void loop();
float readSensor();
float sampling();

void setup() {
  Serial.begin (9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT); //currently testing with LED, will replace with haptic motor
  pinMode(LED,LOW); //currently testing with LED, will replace with haptic motor
}
 
void loop() { 
   

  //calculating the average distance to display from 5 samplings
  float avg_dist = sampling();
  Serial.print(avg_dist);
  Serial.println(" cm");
  //we think that if an object is a metre away,the user should be aware
  //through the haptic motor. We are currently testing with LED
  if(avg_dist < 100.0){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }
  delay(10);
  
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
  delay(60); // A delay of 60ms to allow wave to travel

  // Reading the Echo Pin
  waveTime = pulseIn(ECHO, HIGH);
  // Speed of sound is 343 m/s, use to determine distance from our waveTime
  distance = (waveTime / 2) * 0.0343;
}

//This function calculates the average of five reads and returns it
float sampling(){
  float reads = 0;
  for (int i = 0; i < OVERSMPL; i++){
    curr_dist = readSensor(); //calls the read function once
    //if our distance is in range add to our reads in order to average
    if (curr_dist <= 400 && curr_dist >= 2) {
      reads += curr_dist;
    }
  }
  return reads;
}
