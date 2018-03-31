#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <math.h>
#include "message_filters/subscriber.h"
#include "laser_geometry/laser_geometry.h"

#define X_MIN
#define Y_MIN

#define X_MAX
#define Y_MAX

#define PI 3.14159265

class Obastacle_detector
{
private:
  ros::NodeHandle n_;
  message_filters::Subscriber<sensor_msgs::LaserScan> laser_sub_;
  message_filters::Subscriber<nav_msgs/Odometry> odom_sub_;
  TimeSynchronizer<sensor_msgs::LaserScan, nav_msgs/Odometry> sync;

public:
  Obastacle_detector(ros::NodeHandle n) : n_(n),
    laser_sub_(n_, "scan", 2),
    odom_sub(n_, "odom", 2),
    sync(laser_sub_, odom_sub_, 4)
  {
      sync.registerCallback(boost::bind(&callback, _1, _2);
  }

  float distToBorder(float &x, float &y, float &th)
  {
    float d = 0;

    if(cos(th) > 0)   //Looking to the right
    {
      if(sin(th) > 0) //Looking up
      {
        if((tan(th) * (X_MAX - x) < (Y_MAX - y))
          d = (X_MAX - x) / cos(th);
        else
          d = (Y_MAX - y)/ sin(th);
      }
      else            //Looking down
      {
        if((tan(th) * (X_MAX - x) < (y - Y_MIN))
          d = (X_MAX - x) / cos(th);
        else
          d = -(y - Y_MIN) / sin(th);
      }
    }
    else              //Looking to the left
    {
      if(sin(th) > 0) //Looking up
      {
        if((tan(PI - th) * (x - X_MIN) < (Y_MAX - y))
          d = (x - X_MIN) / cos(PI - th);
        else
          d = (Y_MAX - y)/ sin(PI - th);
      }
      else            //Looking down
      {
        if((tan(PI - th) * (x - X_MIN) < (y - Y_MIN))
          d = -(x - X_MIN) / cos(PI - th);
        else
          d = (y - Y_MIN)/ sin(PI - th);
      }
    }

    return d;

  }

  static void callback(const sensor_msgs::LaserScan::ConstPtr& scan_in,
                       const nav_msgs::Odometry::ConstPtr& odom_in)
  {
    float angle = scan_in->angle_min;
    float increment = scan_in->angle_increment;
    float x = odom_in->pose.pose.position.x;
    float y = odom_in->pose.pose.position.y;
    float theta = odom_in->pose.pose.orientation.

    std::cout << scan_in->ranges.size() << scan_in->angle_min << scan_in->angle_max << std::endl;


    /*for(int i = 0; i < scan_in->ranges.size(); i++)
    {
      angle += scan_in->angle_increment;
      std::cout << "Angle :" << angle << " Dist : " << scan_in->ranges[i] << std::endl;
    }*/
  }
};

int main(int argc, char** argv)
{

  ros::init(argc, argv, "pointcloud_publisher");
  ros::NodeHandle n;
	Obastacle_detector detector(n);

 	ros::spin();

	return 0;
}
