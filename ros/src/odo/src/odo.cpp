#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "comm/Comm.h"

#include <cstdio>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

struct not_digit
{
  bool operator()(const char c)
  {
    return c != ',' && !std::isdigit(c) && c != '-' && c != '.';
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "odo");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<comm::Comm>("pic_pi_comm");
  comm::Comm srv;
  string str;
  char trash;
  not_digit not_a_digit;
  std::string::iterator end;

  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odom_broadcaster;

  srv.request.command = "GETODO\n";

  float x = 0.0;
  float y = 0.0;
  float th = 0.0;

  float v = 0;
  float vth = 0;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(1);
  while(n.ok()){
    if(client.call(srv))
    {
      str = (string)srv.response.answer;
      end = std::remove_if(str.begin(), str.end(), not_a_digit);
      string data(str.begin(), end);
      stringstream ss(data);
      ss >> x >> trash >> y >> trash >> th >> trash >> v >> trash >> vth;
      cout << x << ' ' << y << ' ' << th << std::endl;
    }
    else
    {
      ROS_ERROR("Failed to call service pic_pi_comm");
    }

    ros::spinOnce();               // check for incoming messages
    current_time = ros::Time::now();

    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = v * cos(th);
    odom.twist.twist.linear.y = v * sin(th);
    odom.twist.twist.angular.z = vth;

    //publish the message
    //ROS_INFO("publishing odo");
    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }

  return 0;
}
