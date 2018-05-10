#include <ros/ros.h>
#include <cstdio>
#include "serial/serial.h"
#include <string>
#include "comm/Comm.h"

#define UART_PORT "/dev/ttyAMA0"

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
    if(!uart.isOpen())
    {
      timeout = serial::Timeout::simpleTimeout(100);
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
    size_t nb_char;
    string str;
    string ackStr;
    // step 1: on flush l'uart
    uart.flush();
    // step 2: on envoie l'ordre
    uart.write(req.command);
    //step 3: on attend le resultat
    nb_char = uart.available();
    //step 4: on recupère l'ack
    if(nb_char > 0) {
        uart.readline(ackStr);
        cout << ackStr << endl;
    } else {
        // on a pas recu d'ack, on suppose que l epic n'as rien recu
      cout << "nothing received from pic" << endl;
      ROS_ERROR("Command send to pic didn't gave any response");
        return false;
    }
    // step 5: on lit le resultat de la commande (s'il y en a un )
    nb_char = uart.available();
    if(nb_char > 0) {
        uart.read(str, nb_char);
        cout << str << endl;
    }
    res.answer = str;
    // step 6: on verifie que l'ack est valide
            // remarque: si la commande est malformé ou correspond a une fonction inexistante, le message d'erreur se trouvera dans le resultat (str)
    if (ackStr.find("ACK") == string::npos){
      cout << "The ACK part of the response does not contain the ACK string" << endl;
      ROS_ERROR("The ACK part of the response does not contain the ACK string");
      return false;
    }
    if (str.find("ERROR") != string::npos){
      cout << str << endl;
      ROS_ERROR(str);
      return false;
    }
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
