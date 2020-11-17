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
Servo interface_rotation;
Servo wrist_rotation;
Servo cross_shaft_rotation;
Servo arm_shaft_joint_b;
Servo arm_shaft_joint_a;
Servo rotate_base;

double rotate_base_angle=90;
double arm_shaft_joint_a_angle=90;
double arm_shaft_joint_b_angle=90;
double cross_shaft_rotation_angle=90;
double wrist_rotation_angle=90;
double interface_rotation_angle=90;

double arm_shaft_joint_b_offset=-55;
double cross_shaft_rotation_offset=40;

//define max ranges
double rotate_base_min=0;
double rotate_base_max=180;

double arm_shaft_joint_a_min=45;
double arm_shaft_joint_a_max=150;

double arm_shaft_joint_b_min=30;
double arm_shaft_joint_b_max=160;

double cross_shaft_rotation_min=0;
double cross_shaft_rotation_max=180;

double wrist_rotation_min=15;
double wrist_rotation_max=160;

double interface_rotation_min=10;
double interface_rotation_max=190;

void servo_cb(const sensor_msgs::JointState& cmd_msg){
  rotate_base_angle=radiansToDegrees(cmd_msg.position[0]);

  if(rotate_base_angle < rotate_base_min)
    rotate_base_angle = rotate_base_min;

  if(rotate_base_angle > rotate_base_max)
    rotate_base_angle = rotate_base_max;
  
  arm_shaft_joint_a_angle=radiansToDegrees(-cmd_msg.position[1]);

  if(arm_shaft_joint_a_angle < arm_shaft_joint_a_min)
    arm_shaft_joint_a_angle = arm_shaft_joint_a_min;

  if(arm_shaft_joint_a_angle > arm_shaft_joint_a_max)
    arm_shaft_joint_a_angle = arm_shaft_joint_a_max;
  
  arm_shaft_joint_b_angle=radiansToDegrees(cmd_msg.position[2]+cmd_msg.position[1])+arm_shaft_joint_b_offset;

  if(arm_shaft_joint_b_angle < arm_shaft_joint_b_min)
    arm_shaft_joint_b_angle = arm_shaft_joint_b_min;

  if(arm_shaft_joint_b_angle > arm_shaft_joint_b_max)
    arm_shaft_joint_b_angle = arm_shaft_joint_b_max;
  
  cross_shaft_rotation_angle=radiansToDegrees(cmd_msg.position[3])+cross_shaft_rotation_offset;
  
  if(cross_shaft_rotation_angle < cross_shaft_rotation_min)
    cross_shaft_rotation_angle = cross_shaft_rotation_min;

  if(cross_shaft_rotation_angle > cross_shaft_rotation_max)
    cross_shaft_rotation_angle = cross_shaft_rotation_max;
  
  wrist_rotation_angle=radiansToDegrees(-cmd_msg.position[4]);

  if(wrist_rotation_angle < wrist_rotation_min)
    wrist_rotation_angle = wrist_rotation_min;

  if(wrist_rotation_angle > wrist_rotation_max)
    wrist_rotation_angle = wrist_rotation_max;
  
  interface_rotation_angle=radiansToDegrees(cmd_msg.position[5]);

  if(interface_rotation_angle < interface_rotation_min)
    interface_rotation_angle = interface_rotation_min;

  if(interface_rotation_angle > interface_rotation_max)
    interface_rotation_angle = interface_rotation_max;
  
  rotate_base.write(rotate_base_angle);
  arm_shaft_joint_a.write(arm_shaft_joint_a_angle);
  arm_shaft_joint_b.write(arm_shaft_joint_b_angle);
  cross_shaft_rotation.write(cross_shaft_rotation_angle);
  wrist_rotation.write(wrist_rotation_angle);
  interface_rotation.write(interface_rotation_angle);
  
}


ros::Subscriber<sensor_msgs::JointState> sub("joint_states", servo_cb);

void setup(){
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);

  rotate_base.attach(2);
  arm_shaft_joint_a.attach(3);
  arm_shaft_joint_b.attach(4);
  cross_shaft_rotation.attach(5);
  wrist_rotation.attach(6);
  interface_rotation.attach(7); 

  delay(1);
  rotate_base.write(rotate_base_angle);
  arm_shaft_joint_a.write(arm_shaft_joint_a_angle);
  arm_shaft_joint_b.write(arm_shaft_joint_b_angle+arm_shaft_joint_b_offset);
  cross_shaft_rotation.write(cross_shaft_rotation_angle+cross_shaft_rotation_offset);
  wrist_rotation.write(wrist_rotation_angle);
  interface_rotation.write(interface_rotation_angle);
}

void loop(){
  nh.spinOnce();
}

double radiansToDegrees(float position_radians)
{

  position_radians = position_radians + 1.6;

  return position_radians * 57.2958;

}
