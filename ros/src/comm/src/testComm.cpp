#include <ros/ros.h>
#include <cstdio>
#include <string>
#include "comm/Comm.h"

using namespace std;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "testComm");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<comm::Comm>("pic_pi_comm");

  comm::Comm srv;

  ros::Time temps;

  srv.request.command = "GETODO\n";

  for(int i = 0; i < 5; i++)
  {
    temps = ros::Time::now();
    cout << "temps:" << temps.sec << " " << temps.nsec << endl;
    if(client.call(srv))
    {
      cout << "Answer : " << srv.response.answer << endl;
      temps = ros::Time::now();
      cout << "temps:" << temps.sec << " " << temps.nsec << endl;
    }
    else
    {
      ROS_ERROR("Failed to call service pic_pi_comm");
    }
  }

  return 0;
}
