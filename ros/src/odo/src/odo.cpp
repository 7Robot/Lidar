#include <ros/ros.h>
#include <cstdio>
#include <string>
#include "comm/Comm.h"

using namespace std;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "test_serial");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<comm::Comm>("pic_pi_comm");

  comm::Comm srv;

  srv.request.command = 0;

  if(client.call(srv))
  {
    ROS_INFO("Error : %d", (int)srv.response.error);
  }
  else
  {
    ROS_ERROR("Failed to call service pic_pi_comm");
  }

  return 0;
}
