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
  ss << "MOVESEG " << x << " " << y << endl;
  srv.request.command = ss.str();

  if(client.call(srv))
  {
    //Réponse éventuelle
    str = (string)srv.response.answer;
  }
}

void angle(ros::ServiceClient client, float theta)
{
  comm::Comm srv;
  string str;
  stringstream ss;
  //Action a réaliser sur le pic
  ss << "ANGLE " << theta << endl;
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
			move_xy(_client, 0.22, 0);
  		state++;
  		break;
		case 1:
			if((x_odo > 0.17 && x_odo < 0.27) && (y_odo > -0.05 && y_odo < 0.05))
  			state++;
			break;
		case 2:
 			angle(_client, -2.36);
			state++;
			break;
		case 3:
			move_xy(_client, 0, -0.235);
			state++;
			break;
		case 4:
			if((x_odo > -0.05 && x_odo < 0.05) && (y_odo > -0.285 && y_odo < -0.185))
    		state++;
			break;
		case 5:
 			angle(_client, -2.36);
			state++;
			break;
		case 6: //ramasse les balles de la bonne couleur
			tempo++;
			if(tempo > 40)
			{
  			state++;
  			tempo = 0;
			}
			break;
		case 7:
			move_xy(_client, 0.1, -0.13);
			state++;
			break;
		case 8:
			if((x_odo > 0.05 && x_odo < 0.15) && (y_odo > -0.18 && y_odo < -0.08))
    		state++;
			break;
		case 9:
 			angle(_client, 1.571);
			state++;
			break;
		case 10:
			move_xy(_client, 0.1, 0.05);
			state++;
			break;
		case 11:
			if((x_odo > 0.05 && x_odo < 0.15) && (y_odo > 0 && y_odo < 0.1))
    		state++;
			break;
		case 12:
 			angle(_client, 1.571);
			state++;
			break;
		case 13: //envoie les balles dans la citerne
			tempo++;
			if(tempo > 40)
			{
  			state++;
  			tempo = 0;
			}
			break;
		case 14:
 			angle(_client, -0.786);
			state++;
			break;
		case 15:
			move_xy(_client, 0.61, -0.56);
			state++;
			break;
		case 16:
			if((x_odo > 0.56 && x_odo < 0.66) && (y_odo > -0.61 && y_odo < -0.51))
    		state++;
			break;
		case 17:
 			angle(_client, 0);
			state++;
			break;
		case 18:
			move_xy(_client, 2.14, -0.6);
			state++;
			break;
		case 19:
			if((x_odo > 1.99 && x_odo < 2.19) && (y_odo > -0.65 && y_odo < -0.55))
    		state++;
			break;
		case 20:
 			angle(_client, -0.838);
			state++;
			break;
		case 21:
			move_xy(_client, 2.595, -1.14);
			state++;
			break;
		case 22:
			if((x_odo > 2.545 && x_odo < 2.645) && (y_odo > -1.19 && y_odo < -1.09))
    		state++;
			break;
		case 23:
 			angle(_client, -2.478);
			state++;
			break;
		case 24:
			move_xy(_client, 2.34, -1.34);
			state++;
			break;
		case 25:
			if((x_odo > 2.29 && x_odo < 2.39) && (y_odo > -1.39 && y_odo < -1.29))
    		state++;
			break;
		case 26:
 			angle(_client, -2.478);
			state++;
			break;
		case 27: //ramasse les balles qui sont à trier
			tempo++;
			if(tempo > 40)
			{
  			state++;
  			tempo = 0;
			}
			break;
		case 28:
			move_xy(_client, 2.595, -1.14);
			state++;
			break;
		case 29:
			if((x_odo > 2.545 && x_odo < 2.645) && (y_odo > -1.19 && y_odo < -1.09))
    		state++;
			break;
		case 30:
 			angle(_client, -3.648);
			state++;
			break;
		case 31:
			move_xy(_client, 1.45, -0.52);
			state++;
			break;
		case 32:
			if((x_odo > 1.4 && x_odo < 1.5) && (y_odo > -0.57 && y_odo < -0.47))
    		state++;
			break;
		case 33:
 			angle(_client, 1.571);
			state++;
			break;
		case 34:
			move_xy(_client, 1.45, -1.09);
			state++;
			break;
		case 35:
			if((x_odo > 1.4 && x_odo < 1.5) && (y_odo > -1.14 && y_odo < -1.04))
    		state++;
			break;
		case 36: //dépose balles triées
			tempo++;
			if(tempo > 40)
			{
  			state++;
  			tempo = 0;
			}
			break;
		case 37:
 			angle(_client, 2.443);
			state++;
			break;
		case 38:
			move_xy(_client, 0.085, 0);
			state++;
			break;
		case 39:
			if((x_odo > 0.035 && x_odo < 0.135) && (y_odo > -0.05 && y_odo < 0.05))
    		state++;
			break;
		case 40:
 			angle(_client, 1.571);
			state++;
			break;
		case 41: //Tire balles triées
			tempo++;
			if(tempo > 40)
			{
  			state = 0;
  			tempo = 0;
			}
			break;
	}
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "script_match");
  ros::NodeHandle n;

  _client = n.serviceClient<comm::Comm>("pic_pi_comm");
  ros::Subscriber sub = n.subscribe("odom", 1000, odo_callback);

  //ros::Rate r(5);
  ros::Timer timer = n.createTimer(ros::Duration(0.2), script_callback);

  ros::spin();

  return 0;
}
