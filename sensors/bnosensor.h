#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>
#include <BasicLinearAlgebra.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
sensors_event_t event;

void bnoSetup(){
  bno.begin();
  bno.setExtCrystalUse(true);
}

bool bnoCheck(){
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  return (sys!=0);
}

int bnoCalibration()
{
  uint8_t system, gyro, accel, mg = 0;
  bno.getCalibration(&system, &gyro, &accel, &mg);
  if (gyro == 3 && accel == 3)
      return(0);
  return (int) (accel+1);
}

void bnoReading(){
  bno.getEvent(&event);
}

void bnoGetValues(){
  bno.getEvent(&event);
  float tx = (float)event.orientation.x;
  float ty = (float)event.orientation.y;
  float tz = (float)event.orientation.z;
  
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bnoValid = true;
  if ( ty == 0.00 && tx == 0.00 && tz == 0.00 ){
    if ( bnoCheck() == 0 ){
        bnoValid = false;
        bno.begin();
        smartDelay(100);
    }
  }
  acceleration = sqrt(acc.x() * acc.x() + acc.y() * acc.y() + acc.z() * acc.z());
  xAngle = tx;
  yAngle = ty;
  zAngle = tz;
}

void bnoGetXAcc(float *xAcc)
{
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  *xAcc = acc.x();
}

void bnoGetYAcc(float *yAcc)
{
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  *yAcc = acc.y();
}

void bnoGetZAcc(float *zAcc)
{
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  *zAcc = acc.z();
}



void upacceleration(){
 imu::Vector<3> ACCE = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  imu::Vector<3> GRAV = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
double gravitationatdesti=9.81;
double ax=GRAV.x()/gravitationatdesti;
double ay=GRAV.y()/gravitationatdesti;
double az=GRAV.z()/gravitationatdesti;
BLA::Matrix<3, 1> accelerationbno={ACCE.x(),ACCE.y(),ACCE.z()};
//BLA::Matrix<3, 1> accelerationbno={GRAV.x(),GRAV.y(),GRAV.z()};
BLA::Matrix<3, 3> RMGRAV = {(ay*ay-ax*ax*az)/(ax*ax+ay*ay),(-ax*ay-ax*ay*az)/(ax*ax+ay*ay),ax,
                           (-ax*ay-ax*ay*az)/(ax*ax+ay*ay), (ax*ax-ay*ay*az)/(ax*ax+ay*ay),ay,
                           -ax,-ay,-az};
//BLA::Matrix<3, 3> INRMa=Invert(RMGRAV);
BLA::Matrix<3, 1> rotatedacc=RMGRAV*accelerationbno;
  
}
