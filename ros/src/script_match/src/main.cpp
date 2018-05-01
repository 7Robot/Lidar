#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <tf/LinearMath/Quaternion.h>
#include <string>
#include "comm/Comm.h"

using namespace std;

float x_odo = 0;
float y_odo = 0;
float theta_odo = 0;

ros::ServiceClient _client; 


void odo_callback(const nav_msgs::Odometry::ConstPtr& odom_in)
{
  x_odo = odom_in->pose.pose.position.x;
  y_odo = odom_in->pose.pose.position.y;
  tf::Quaternion q(odom_in->pose.pose.orientation.x,
                   odom_in->pose.pose.orientation.y,
                   odom_in->pose.pose.orientation.z,
                   odom_in->pose.pose.orientation.w);
  tf::Matrix3x3 m(q);
  double theta;
  double trash;
  m.getRPY(trash, trash, theta);
  theta_odo = theta;
  std::cout << x_odo << " " << y_odo << endl;
}

void move_xy(ros::ServiceClient client, float x, float y)
{
  comm::Comm srv;
  string str;
  stringstream ss;
  //Action a réaliser sur le pic
  ss << "MOVE " << x << " " << y << endl;
  srv.request.command = ss.str();

  if(client.call(srv))
  {
    //Réponse éventuelle
    str = (string)srv.response.answer;
  }
}

void script_callback(const ros::TimerEvent& trash)
{
  static int state = 0;
  static int tempo = 0;
  
  switch(state)
  {
    case 0:
      move_xy(_client, 0.5, 0);
      state++;
      break;
    case 1:
      if((x_odo > 0.4 && x_odo < 0.6) && (y_odo > -0.1 && y_odo < 0.1))
        state++;
      break;
    case 2:
      move_xy(_client, 0, 0);
      state++;
      break;
    case 3:
      tempo++;
      if(tempo > 50)
      {
        state++;
	tempo = 0;
      }
      break;
    case 4:
      if((x_odo > -0.1 && x_odo < 0.1) && (y_odo > -0.1 && y_odo < 0.1))
        state++;
      break;
  }
  std::cout << state << std::endl;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "script_match");
  ros::NodeHandle n;

  _client = n.serviceClient<comm::Comm>("pic_pi_comm");
  ros::Subscriber sub = n.subscribe("odom", 1000, odo_callback);

  //ros::Rate r(5);
  ros::Timer timer = n.createTimer(ros::Duration(0.2), script_callback);

  /*while(n.ok())
  {
    switch(state)
    {
    case 0:
      move_xy(client, 0.5, 0);
      state++;
      break;
    case 1:
      if((x_odo > 0.49 && x_odo < 0.51) && (y_odo > -0.01 && y_odo < 0.01))
        state++;
      break;
    case 2:
      move_xy(client, 0, 0);
      state++;
      break;
    case 3:
      if((x_odo > -0.1 && x_odo < 0.1) && (y_odo > -0.1 && y_odo < 0.1))
        state++;
      break;
    }

    r.sleep();
    //std::cout << x_odo << " " << y_odo << std::endl; 
  }*/

  ros::spin();

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
