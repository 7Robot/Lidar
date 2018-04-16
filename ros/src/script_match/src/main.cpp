#include <ros/ros.h>
#include "comm/Comm.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "odo");
  ros::NodeHandle n;

  ros::ServiceClient client = n.serviceClient<comm::Comm>("pic_pi_comm");

  //Example
  foo(client, autre params)


  return 0;
}

void foo(ros::ServiceClient client, autres params)
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
}
