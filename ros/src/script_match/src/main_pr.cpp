#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <tf/LinearMath/Quaternion.h>

#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "comm/Comm.h"

#define pi 3.14159

using namespace std;

/*float x_odo = 0;
float y_odo = 0;
float theta_odo = 0;*/

ros::ServiceClient _client;

struct not_digit
{
  bool operator()(const char c)
  {
    return c != ',' && !std::isdigit(c) && c != '-' && c != '.';
  }
};

void get_odo(ros::ServiceClient client, float &x, float &y, float &theta)
{
  comm::Comm srv;
  string str;
  char trash;

  not_digit not_a_digit;
  std::string::iterator fin;

  srv.request.command = "GETODO\n";

  if(client.call(srv))
  {
    str = (string)srv.response.answer;
    fin = std::remove_if(str.begin(), str.end(), not_a_digit);
    string data(str.begin(), fin);
    stringstream ss(data);
    ss >> x >> trash >> y >> trash >> theta;
    //cout << x << ' ' << y << ' ' << th << std::endl;
  }
  else
  {
    ROS_ERROR("Failed to call service pic_pi_comm");
  }
  /*x_odo = odom_in->pose.pose.position.x;
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
  //std::cout << x_odo << " " << y_odo << endl;*/
}

//////////////////////////// Actions AX12 ////////////////////////////
bool check_answer(ros::ServiceClient client, comm::Comm srv)
{
  bool ok = false;
  string str;

  if(client.call(srv))
  {
    //Réponse éventuelle
    str = (string)srv.response.answer;
    if(str=="STATE:0")
      ok = true;
  }
  return ok;
}

bool do_AX12action(ros::ServiceClient client, string command)
{
  comm::Comm srv;
  string str;
  bool ok = false;
  str = command;

  srv.request.command = str;

  return check_answer(client, srv);
}
/////////////////////////////////////////////////////////////////////

bool move_xy(ros::ServiceClient client, float x, float y)
{
  comm::Comm srv;
  string str;
  stringstream ss;
  //Action a réaliser sur le pic
	ss << fixed;
	ss.precision(4);
  ss << "MOVESEG " << x << " " << y << endl;
  srv.request.command = ss.str();

  return check_answer(client, srv);
}

bool angle(ros::ServiceClient client, float theta)
{
  comm::Comm srv;
  string str;
  stringstream ss;
  //Action a réaliser sur le pic
	ss << fixed;
	ss.precision(4);
  ss << "ANGLE " << theta << endl;
  srv.request.command = ss.str();

  return check_answer(client, srv);
}

void script_callback(const ros::TimerEvent& trash)
{
	static int state = 0;
	static int tempo = 0;

  float x_odo = 0;
  float y_odo = 0;
  float theta_odo = 0;

  ros::Time date;
  date = ros::Time::now();

  get_odo(_client, x_odo, y_odo, theta_odo);

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
      if(theta_odo > -2.4 && theta_odo < -2.3)
        state++;
      break;
    case 4:
			move_xy(_client, 0, -0.235);
			state++;
			break;
		case 5:
			if((x_odo > -0.05 && x_odo < 0.05) && (y_odo > -0.285 && y_odo < -0.185))
    		state++;
			break;
		case 6:
 			angle(_client, -2.36);
			state++;
			break;
		case 7: //ramasse les balles de la bonne couleur
      //do_AX12action(_client, "TRICANON\n");
      state++;
      break;
    case 8:
      //if(do_AX12action(_client, "OVERTRICANON\n"))
      state++;
      break;
		case 9:
			move_xy(_client, 0.1, -0.13);
			state++;
			break;
		case 10:
			if((x_odo > 0.01 && x_odo < 0.15) && (y_odo > -0.18 && y_odo < -0.08))
    		state++;
			break;
		case 11:
 			angle(_client, 1.571);
			state++;
			break;
		case 12:
			move_xy(_client, 0.1, 0.05);
			state++;
			break;
		case 13:
			if((x_odo > 0.05 && x_odo < 0.15) && (y_odo > 0 && y_odo < 0.15))
    		state++;
			break;
		case 14:
 			angle(_client, 1.571);
			state++;
			break;
		case 15: //envoie les balles dans la citerne
      //do_AX12action(_client, "TURBON\n");
      state++;
			break;
    case 16:
      // if(do_AX12action(_client, "OVERTURBON\n"))
      // do_AX12action(_client, "OUVRIRCANON\n");
      state++;
      break;
    case 17:
      // Tempo
      tempo++;
      if(tempo > 20)
      {
        state++;
        tempo = 0;
      }
      break;
    case 18:
      // do_AX12action(_client, "TURBOFF\n");
      // if(do_AX12action(_client, "OVERTURBOFF\n"))
      state++;
      break;
    case 19:
      // do_AX12action(_client, "FERMERCANON\n")
      // if(do_AX12action(_client, "OVERFERMERCANON\n"))
      state++;
      break;
		case 20:
 			angle(_client, -0.786);
			state++;
			break;
		case 21:
			move_xy(_client, 0.61, -0.56);
			state++;
			break;
		case 22:
			if((x_odo > 0.56 && x_odo < 0.66) && (y_odo > -0.61 && y_odo < -0.51))
    		state++;
			break;
		case 23:
 			angle(_client, 0);
			state++;
			break;
		case 24:
			move_xy(_client, 2.14, -0.6);
			state++;
			break;
		case 25:
			if((x_odo > 1.99 && x_odo < 2.19) && (y_odo > -0.65 && y_odo < -0.55))
    		state++;
			break;
		case 26:
 			angle(_client, -0.838);
			state++;
			break;
		case 27:
			move_xy(_client, 2.595, -1.14);
			state++;
			break;
		case 28:
			if((x_odo > 2.545 && x_odo < 2.645) && (y_odo > -1.19 && y_odo < -1.09))
    		state++;
			break;
		case 29:
 			angle(_client, -2.478);
			state++;
			break;
		case 30:
			move_xy(_client, 2.34, -1.34);
			state++;
			break;
		case 31:
			if((x_odo > 2.29 && x_odo < 2.39) && (y_odo > -1.39 && y_odo < -1.29))
    		state++;
			break;
		case 32:
 			angle(_client, -2.478);
			state++;
			break;
		case 33: //ramasse les balles qui sont à trier
      // do_AX12action(_client, "TRIALT\n");
      state++;
      break;
    case 34:
      //if(do_AX12action(_client, "OVERTRIALT\n"))
			tempo++;
			if(tempo > 20)
			{
  			state++;
  			tempo = 0;
			}
			break;
		case 35:
			move_xy(_client, 2.595, -1.14);
			state++;
			break;
		case 36:
			if((x_odo > 2.545 && x_odo < 2.645) && (y_odo > -1.19 && y_odo < -1.09))
    		state++;
			break;
		case 37:
 			angle(_client, -3.648);
			state++;
			break;
		case 38:
			move_xy(_client, 1.45, -0.52);
			state++;
			break;
		case 39:
			if((x_odo > 1.4 && x_odo < 1.5) && (y_odo > -0.57 && y_odo < -0.47))
    		state++;
			break;
		case 40:
 			angle(_client, 1.571);
			state++;
			break;
		case 41:
			move_xy(_client, 1.45, -1.09);
			state++;
			break;
		case 42:
			if((x_odo > 1.4 && x_odo < 1.5) && (y_odo > -1.14 && y_odo < -1.04))
    		state++;
			break;
		case 43: //dépose balles triées
      //do_AX12action(_client, "OUVRIRTRAPPE\n");
      state++;
      break;
    case 44:
      //if(do_AX12action(_client, "OVEROUVRIRTRAPPE\n"))
      state++;
      break;
    case 45:
      //tempo pour que les balles tombent
			tempo++;
			if(tempo > 20)
			{
  			state++;
  			tempo = 0;
			}
			break;
    case 46:
      //do_AX12action(_client, "FERMERTRAPPE\n");
      state++;
      break;
    case 47:
      //if(do_AX12action(_client, "OVERFERMERTRAPPE\n"))
      state++;
      break;
		case 48:
 			angle(_client, 2.443);
			state++;
			break;
		case 49:
			move_xy(_client, 0.085, 0);
			state++;
			break;
		case 50:
			if((x_odo > 0.035 && x_odo < 0.135) && (y_odo > -0.05 && y_odo < 0.05))
    		state++;
			break;
		case 51:
 			angle(_client, 1.571);
			state++;
			break;
		case 52: //Tire balles triées
      //do_AX12action(_client, "TURBON\n");
      state++;
      break;
    case 53:
      //if(do_AX12action(_client, "OVERTURBON\n"))
      state++;
      break;
    case 54:
      //do_AX12action(_client, "OUVRIRCANON\n");
      state++;
      break;
    case 55:
      //if(do_AX12action(_client, "OVEROUVRIRCANON\n"))
      state++;
      break;
    case 56:
      // Tempo le temps de lancer les balles
			tempo++;
			if(tempo > 20)
			{
  			state++;
  			tempo = 0;
			}
			break;
    case 57:
      //do_AX12action(_client, "FERMERCANON\n");
      state++;
      break;
    case 58:
      //if(do_AX12action(_client, "OVERFERMERCANON\n"))
      state = 0;
      break;
	}
  std::cout << date.sec << " " << date.nsec << " " << x_odo << " " << y_odo << " " << theta_odo
  << " " << state << std::endl;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "script_match");
  ros::NodeHandle n;
  
  _client = n.serviceClient<comm::Comm>("pic_pi_comm");
  //do_AX12action(_client, "AX12INIT\n");
  //ros::Subscriber sub = n.subscribe("odom", 1000, odo_callback);

  //ros::Rate r(5);
  //if (do_AX12action(_client, "OVERAX12INIT\n"))
  ros::Timer timer = n.createTimer(ros::Duration(0.5), script_callback);

  ros::spin();

  return 0;
}
