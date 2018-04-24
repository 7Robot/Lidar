#include <iostream>
#include <math.h>
#include "comm/Comm.h"

#include <ros/ros.h>

#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

#include <laser_geometry/laser_geometry.h>
#include <nav_msgs/Odometry.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <tf/LinearMath/Quaternion.h>

#define X_MIN 0
#define Y_MIN 0

#define X_MAX 5
#define Y_MAX 3

#define PI 3.14159265

typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::LaserScan, nav_msgs::Odometry> sync_policy;

ros::ServiceClient client;
comm::Comm srv;

class Obastacle_detector
{
private:
  ros::NodeHandle n_;
  message_filters::Subscriber<sensor_msgs::LaserScan> laser_sub_;
  message_filters::Subscriber<nav_msgs::Odometry> odom_sub_;
  message_filters::Synchronizer<sync_policy> sync;

public:
  Obastacle_detector(ros::NodeHandle n) : n_(n),
    laser_sub_(n_, "scan", 2),
    odom_sub_(n_, "odom", 2),
    sync(sync_policy(10), laser_sub_, odom_sub_)
  {
    sync.registerCallback(boost::bind(&callback, _1, _2));
  }

  static float distToBorder(float &x, float &y, float &th)
  {
    float d = 0;

    if(cos(th) > 0)   //Looking to the right
    {
      if(sin(th) > 0) //Looking up
      {
        if((tan(th) * (X_MAX - x)) < (Y_MAX - y))
          d = (X_MAX - x) / cos(th);
        else
          d = (Y_MAX - y)/ sin(th);
      }
      else            //Looking down
      {
        if((tan(th) * (X_MAX - x)) < (y - Y_MIN))
          d = (X_MAX - x) / cos(th);
        else
          d = -(y - Y_MIN) / sin(th);
      }
    }
    else              //Looking to the left
    {
      if(sin(th) > 0) //Looking up
      {
        if((tan(PI - th) * (x - X_MIN)) < (Y_MAX - y))
          d = (x - X_MIN) / cos(PI - th);
        else
          d = (Y_MAX - y)/ sin(PI - th);
      }
      else            //Looking down
      {
        if((tan(PI - th) * (x - X_MIN)) < (y - Y_MIN))
          d = (x - X_MIN) / cos(PI - th);
        else
          d = -(y - Y_MIN)/ sin(PI - th);
      }
    }

    return d;

  }

  static void callback(const sensor_msgs::LaserScan::ConstPtr& scan_in,
                       const nav_msgs::Odometry::ConstPtr& odom_in)
  {
    /*float x = odom_in->pose.pose.position.x;
    float y = odom_in->pose.pose.position.y;
    tf::Quaternion q(odom_in->pose.pose.orientation.x,
                     odom_in->pose.pose.orientation.y,
                     odom_in->pose.pose.orientation.z,
                     odom_in->pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    double theta = 0;
    double trash;
    m.getRPY(trash, trash, theta);*/
    float angle = scan_in->angle_min;// + theta;
    float increment = scan_in->angle_increment;
    bool obstacle = false;
    bool p_obstacle = false;

    for(int i = 319; i < 502; i += 2)
    {
      angle += 2 * scan_in->angle_increment;
      //d = distToBorder(x, y, angle);
      if(scan_in->ranges[i] < 0.25)
      {
        obstacle = true;
      }

    }
    if(obstacle)
    {
      srv.request.command = "STOP\n";
      client.call(srv);
      p_obstacle = true;
    }
      //std::cout << odom_in->header.stamp.sec << " " << scan_in->header.stamp.sec << " Obstacle" << std::endl;
    else
    {
      if(p_obstacle)
      {
        srv.request.command = "RESTART\n";
        client.call(srv);
        p_obstacle = false;
      }
    }
  }
};

int main(int argc, char** argv)
{

  ros::init(argc, argv, "pointcloud_publisher");
  ros::NodeHandle n;
  client = n.serviceClient<comm::Comm>("pic_pi_comm");
	Obastacle_detector detector(n);

 	ros::spin();

	return 0;
}
