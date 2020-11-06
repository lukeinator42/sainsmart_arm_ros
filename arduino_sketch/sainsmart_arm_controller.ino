#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include <WProgram.h>
#endif

#include <Servo.h> 
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <sensor_msgs/JointState.h>

ros::NodeHandle  nh;
Servo gripper;
Servo wrist;
Servo elbow;
Servo shoulder2;
Servo shoulder1;
Servo base;

double base_angle=90;
double shoulder_angle=90;
double elbow_angle=90;
double wrist_angle=90;
double gripper_angle=90;

void servo_cb(const sensor_msgs::JointState& cmd_msg){
  base_angle=radiansToDegrees(cmd_msg.position[0]);
  shoulder_angle=radiansToDegrees(cmd_msg.position[1]);
  elbow_angle=radiansToDegrees(cmd_msg.position[2]);
  wrist_angle=radiansToDegrees(cmd_msg.position[3]);
  gripper_angle=radiansToDegrees(cmd_msg.position[4]);
  
  base.write(base_angle);
  shoulder1.write(shoulder_angle);
  shoulder2.write(shoulder_angle);
  elbow.write(elbow_angle);
  wrist.write(wrist_angle);
  gripper.write(gripper_angle-15);
  
}


ros::Subscriber<sensor_msgs::JointState> sub("joint_states", servo_cb);

void setup(){
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);

  base.attach(2); 
  gripper.attach(8); 
  wrist.attach(11);
  elbow.attach(6); 
  shoulder2.attach(4);
  shoulder1.attach(3); 

  delay(1);
  base.write(90);
  shoulder1.write(90);
  shoulder2.write(90);
  elbow.write(90);
  wrist.write(90);
  gripper.write(90);
}

void loop(){
  nh.spinOnce();
}

double radiansToDegrees(float position_radians)
{

  position_radians = position_radians + 1.6;

  return position_radians * 57.2958;

}
