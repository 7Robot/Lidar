#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <tf/LinearMath/Quaternion.h>
#include "comm/Comm.h"

float x_odo;
float y_odo;
float theta_odo;

void odo_callback(const nav_msgs::Odometry::ConstPtr& odom_in)
{
  x_odo = odom_in->pose.pose.position.x;
  y_odo = odom_in->pose.pose.position.y;
  tf::Quaternion q(odom_in->pose.pose.orientation.x,
                   odom_in->pose.pose.orientation.y,
                   odom_in->pose.pose.orientation.z,
                   odom_in->pose.pose.orientation.w);
  tf::Matrix3x3 m(q);
  double trash;
  m.getRPY(trash, trash, theta_odo);
}

void move_xy(ros::ServiceClient client, float x, float y)
{
  comm::Comm srv;
  string str;
  //Action a réaliser sur le pic
  str = "MOVE " + x + " " + y;
  srv.request.command = str;

  if(client.call(srv))
  {
    //Réponse éventuelle
    str = (string)srv.response.answer;
  }
}
int main(int argc, char** argv)
{
  int state = 0;
  ros::init(argc, argv, "script_match");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<comm::Comm>("pic_pi_comm");

  ros::Subscriber sub = n.subscribe("odom", 1000, odo_callback);

  ros::Rate r(5);

  while(n.ok())
  {
    switch(state)
    {
    case 0:
      move_xy(client, 0.5, 0);
      state++;
      break;
    case 1:
      if((x_odo > 0.49 || x_odo < 0.51) && (y_odo > -0.01 || y_odo < 0.01))
        state++;
      break;
    case 2:
      move_xy(client, 0, 0);
      state++;
      break;
    case 3:
      if((x_odo > -0.1 || x_odo < 0.1) && (y_odo > -0.1 || y_odo < 0.1))
        state++;
      break;
    }

    r.sleep();
  }

  return 0;
}

/*void foo(ros::ServiceClient client, autres params)
{
  comm::Comm srv;
  string str;
  //Action a réaliser sur le pic
  str = "MY_CMD\n";
  //Ajout des param éventuelles

  srv.request.command = str;

  if(client.call(srv))
  {
    //Réponse éventuelle
    str = (string)srv.response.answer;
  }
}*/
