#include <ros/ros.h>
#include <cstdio>
#include "serial/serial.h"
#include <string>
#include "comm/Comm.h"

#define UART_PORT "/dev/ttyUSB0"

using namespace std;

class PicCommunication{
private:
  static serial::Serial uart;
public:
  PicCommunication()
  {

  }

  static int init()
  {
    ROS_INFO("Starting comm server");

    if(!uart.isOpen())
    {
      uart.setPort(UART_PORT);
      uart.setBaudrate(57600);
      uart.open();
      if(!uart.isOpen())
      {
        ROS_ERROR("Error while opening!");
        exit(-1);
      }
    }
  }

  static bool interpret( comm::Comm::Request &req,
                  comm::Comm::Response &res)
  {
    string str;
    res.error = 0;

    //TODO Code à completer
    switch(req.command)
    {
    case 0:
      uart.write("GETPOS\n");
      cout << uart.read(512) << endl;
      break;
    default:
      res.error = -1;
    }

    return true;
  }
};

serial::Serial PicCommunication::uart;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "comm_server");
  ros::NodeHandle n;

  PicCommunication::init();

  ros::ServiceServer service = n.advertiseService("pic_pi_comm", PicCommunication::interpret);

  ros::spin();

  return 0;
}
