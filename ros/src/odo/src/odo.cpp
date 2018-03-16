#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "comm/Comm.h"

#include <cstdio>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

void split(string s)
{
  vector<string> result;
  string delimiter1 = ",";
  string delimiter2 = ":";
  size_t pos = 0;
  string token;

  while ((pos = s.find(delimiter)) != string::npos)
  {
      token = s.substr(0, pos);
      result.push_back(token);
      s.erase(0, pos + delimiter.length());
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "test_serial");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<comm::Comm>("pic_pi_comm");
  comm::Comm srv;
  char str[45];
  char trash[5];

  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odom_broadcaster;

  srv.request.command = "GETODO\n";

  float x = 0.0;
  float y = 0.0;
  float th = 0.0;

  float v = 0;
  double vx = 0;
  double vy = -0;
  float vth = 0;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(3.0);
  while(n.ok()){
    if(client.call(srv))
    {
      split((string)srv.response.answer);
      strcpy(str, ((string)srv.response.answer).c_str());
      cout << str << endl;
      sscanf(str, "%s %f %s %f %s %f %s %f", trash, &x, &y, &th, &v, &vth);
      cout << x << ' ' << y << ' ' << th << ' ' << v << ' ' << vth << endl;
    }
    else
    {
      ROS_ERROR("Failed to call service pic_pi_comm");
    }

    ros::spinOnce();               // check for incoming messages
    current_time = ros::Time::now();

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;

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
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //publish the message
    //ROS_INFO("publishing odo");
    odom_pub.publish(odom);

    last_time = current_time;
    r.sleep();
  }

  return 0;
}
