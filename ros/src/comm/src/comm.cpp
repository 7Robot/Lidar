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
  static serial::Timeout timeout;
public:
  PicCommunication()
  {

  }

  static int init()
  {
    ROS_INFO("Starting comm server");

    if(!uart.isOpen())
    {
      timeout = serial::Timeout::simpleTimeout(1000);
      uart.setPort(UART_PORT);
      uart.setBaudrate(57600);
      uart.setTimeout(timeout);
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
    uart.flush();
    uart.write(req.command);
    str = uart.read(256);
    res.answer = str;

    return true;
  }
};

serial::Serial PicCommunication::uart;
serial::Timeout PicCommunication::timeout;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "comm");
  ros::NodeHandle n;

  PicCommunication::init();

  ros::ServiceServer service = n.advertiseService("pic_pi_comm", PicCommunication::interpret);

  ros::spin();

  return 0;
}
