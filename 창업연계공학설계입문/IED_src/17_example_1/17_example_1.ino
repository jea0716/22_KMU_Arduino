#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
#define PIN_IR 0 // IR at Pin A0
// Add IR Sensor Definition Here !!!
#define PIN_LED   9   // LED active-low
#define PIN_SERVO 10

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 250.0   // maximum distance to be measured (unit: mm)

#define LOOP_INTERVAL 50   // Loop Interval (unit: msec)

#define _EMA_ALPHA 0.6

Servo myservo;
unsigned long last_loop_time;   // unit: msec
float dist, dist_prev, dist_ema;



void setup()
{
  pinMode(PIN_LED, OUTPUT);
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);

  dist_prev = _DIST_MIN; // raw distance output from USS (unit: mm)
  dist_ema = _DIST_MIN;

  Serial.begin(57600);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  // a_value = analogRead(PIN_POTENTIOMETER);
  // Read IR Sensor value !!!
  // a_value = (analogRead(PIN_IR) / 1024.0) * 5.0;
  a_value = analogRead(PIN_IR);

  // Convert IR sensor value into distance !!!
  dist = (6762.0 / (a_value - 9) - 4.0) * 10.0 - 60.0;

  // we need distance range filter here !!!
  if (dist < _DIST_MIN) {
    dist = dist_prev - 50;           // cut lower than minimum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (dist > _DIST_MAX) {
    dist = dist_prev + 50;           // Cut higher than maximum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON      
    dist_prev = dist;
  }
  
  // we need EMA filter here !!!
  dist_ema = (_EMA_ALPHA) * (dist) + (1 -_EMA_ALPHA) * dist_ema;

  // map distance into duty
  // duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  // myservo.writeMicroseconds(duty);

  if(dist_ema < _DIST_MIN){
    duty = _DUTY_MIN;
    myservo.writeMicroseconds(_DUTY_MIN);
  }
  else if(dist_ema > _DIST_MAX){
    duty = _DUTY_MAX;
    myservo.writeMicroseconds(_DUTY_MAX);
  }
  else{
    duty = _DUTY_MIN + ((_DUTY_MAX - _DUTY_MIN) / 180.0) * (dist_ema - 100);
    myservo.writeMicroseconds(duty);
  }

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN: "); Serial.print(_DIST_MIN);
  Serial.print(", IR: "); Serial.print(a_value);
  Serial.print(", dist: "); Serial.print(dist);
  Serial.print(", ema: "); Serial.print(dist_ema);
  Serial.print(", servo: "); Serial.print(duty);
  Serial.print(", MAX: "); Serial.print(_DIST_MAX);
  Serial.println("");
  // Serial.print("ADC Read: "); Serial.print(a_value);
  // Serial.print(" = ");
  // Serial.print((a_value / 1024.0) * 5.0);
  // Serial.print(" Volt => Duty : ");
  // Serial.print(duty);
  // Serial.println("usec");
}