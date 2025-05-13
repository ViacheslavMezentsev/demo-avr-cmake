<<<<<<< HEAD
/* 
 * rosserial::std_msgs::Time Test
 * Publishes current time
 */

#include <ros.h>
#include <ros/time.h>
#include <std_msgs/Time.h>


ros::NodeHandle  nh;

std_msgs::Time test;
ros::Publisher p("my_topic", &test);

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.advertise(p);
}

void loop()
{  
  test.data = nh.now();
  p.publish( &test );
  nh.spinOnce();
  delay(10);
}

=======
/* 
 * rosserial::std_msgs::Time Test
 * Publishes current time
 */

#include <ros.h>
#include <ros/time.h>
#include <std_msgs/Time.h>


ros::NodeHandle  nh;

std_msgs::Time test;
ros::Publisher p("my_topic", &test);

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.advertise(p);
}

void loop()
{  
  test.data = nh.now();
  p.publish( &test );
  nh.spinOnce();
  delay(10);
}

>>>>>>> 5881ee9d9a49cdc272890e0007b9baca97e186f3
