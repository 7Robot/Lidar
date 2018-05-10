#include <ros/ros.h>
#include <laser_geometry/laser_geometry.h>
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
// DETECTLIDAR 1 -> devant 0 -> arriere
//ENLIDAR 0 pour activer evitement 1 pour desactiver
ros::ServiceClient _client;

int lidar_enable_av = 1;
int lidar_enable_ar = 1;

struct not_digit
{
  bool operator()(const char c)
  {
    return c != ',' && !std::isdigit(c) && c != '-' && c != '.';
  }
};

string getStart(ros::ServiceClient client);


void lidar_callback(const sensor_msgs::LaserScan::ConstPtr& scan_in)
{
  float angle = scan_in->angle_min;
  float inc = scan_in->angle_increment;
  int obstacle_av = 0;
  int obstacle_ar = 0;
  static bool p_obstacle_av = false;
  static bool p_obstacle_ar = false;
  static int cpt_obstacle = 0;

  ros::Time date;
  date = ros::Time::now();

  comm::Comm srv;

  //Check devant
  for(int i = 592; i < 788; i += 2)
  {
    if(scan_in->ranges[i] < 0.35 && lidar_enable_av)
    {
       obstacle_av++;
    }
  }

  //Check derriere
  for(int i = 22; i < 218; i += 2)
  {
    if(scan_in->ranges[i] < 0.35 && lidar_enable_ar)
    {
      obstacle_ar++;
    }
  }

  //Analyze resultat
  if((obstacle_av || obstacle_ar))
  {
    if(obstacle_av > 3 && !p_obstacle_av)
    {
	srv.request.command = "STOP\n";
	if(_client.call(srv))
		;
     cout << date.sec << " " << date.nsec << " obstacle av"  << endl;
      p_obstacle_av = true;
    }
    if(obstacle_ar > 3 && !p_obstacle_ar)
    {
	srv.request.command = "STOP\n";
	if(_client.call(srv))
		;
    cout << date.sec << " " << date.nsec << " obstacle ar" << endl;
      p_obstacle_ar = true;
    }
  }


    if((p_obstacle_av || p_obstacle_ar) && (obstacle_av < 3 && obstacle_ar < 3))
    {
      p_obstacle_av = false;
      p_obstacle_ar = false;
      srv.request.command = "RESTART\n";
	if(_client.call(srv))
		;
      cout << date.sec << " " << date.nsec << " fin obstacle" << endl;
    }
    /*if(p_obstacle_ar && obstacle_ar < 3)
    {
      p_obstacle_ar = false;
      srv.request.command = "RESTART\n";
	if(_client.call(srv))
		;
      cout << date.sec << " " << date.nsec << " fin obstacle ar" << endl;
    }*/

}

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
  }
  else
  {
    ROS_ERROR("Failed to call service pic_pi_comm");
  }
}

//////////////////////////// Actions AX12 ////////////////////////////
bool check_AX12action(ros::ServiceClient client, string command)
{
  comm::Comm srv;
  bool ok = false;
  string str;

  srv.request.command = command;

  if(client.call(srv))
  {
    //Réponse éventuelle
    str = (string)srv.response.answer;
    if(str.find("STATE:0") != string::npos)
      ok = true;
  }
  return ok;
}

bool do_AX12action(ros::ServiceClient client, string command)
{
  comm::Comm srv;
  bool etatComm = false;

  srv.request.command = command;

  if(client.call(srv))
    etatComm = true;
  return etatComm;
}
/////////////////////////////////////////////////////////////////////

bool check_position(ros::ServiceClient client)
{
  comm::Comm srv;
  bool asservDone = false;
  string str;

  srv.request.command = "DONE\n";

  if(client.call(srv))
  {
    str = (string)srv.response.answer;
    if(str.find("1") != string::npos)
      asservDone = true;
  }
  return asservDone;
}

bool move_xy(ros::ServiceClient client, float x, float y)
{
  comm::Comm srv;
  bool ok = false;
  string str;
  stringstream ss;
  //Action a réaliser sur le pic
	ss << fixed;
	ss.precision(4);
  ss << "MOVESEG " << x << " " << y << endl;

  srv.request.command = ss.str();

  if(client.call(srv))
  {
    str = (string)srv.response.answer;
    if(str.find("STATE:0") != string::npos)
      ok = true;
  }
  return ok;
}

bool angle(ros::ServiceClient client, float theta)
{
  comm::Comm srv;
  bool ok = false;
  string str;
  stringstream ss;
  //Action a réaliser sur le pic
	ss << fixed;
	ss.precision(4);
  ss << "ANGLE " << theta << endl;
  srv.request.command = ss.str();

  if(client.call(srv))
  {
    str = (string)srv.response.answer;
    if(str.find("STATE:O") != string::npos)
      ok = true;
  }
  return ok;
}

void cote_orange()
{
  static int state = 0;

  switch(state)
  {
    //le switch ici
  }
}

void cote_vert()
{
  static int state = 0;
  static int tempo = 0;
  string start;

  switch (state)
  {
    case 0:
    	start = getStart(_client);
      if(start.find("MATCH:1") != string::npos)
        state++;
    	break;
    case 1:
      move_xy(_client, 0.22, 0);
  		state = 10;
  		break;
  	case 10:
  		//if((x_odo > 0.17 && x_odo < 0.27) && (y_odo > -0.05 && y_odo < 0.05))
      if(check_position(_client))
  			state = 20;
  		break;
  	case 20:
  		lidar_enable_ar = 0;
  		lidar_enable_av = 0;
  		angle(_client, -2.36);
  		state = 30;
  		break;
  	case 30:
      //if(theta_odo > -2.4 && theta_odo < -2.3)
      if(check_position(_client))
        state = 40;
      break;
    case 40:
  		lidar_enable_ar = 1;
  		move_xy(_client, 0, -0.235);
  		state = 50;
  		break;
  	case 50:
  		//if((x_odo > -0.05 && x_odo < 0.05) && (y_odo > -0.285 && y_odo < -0.185))
      if(check_position(_client))
    		state = 60;
  	  break;
    case 60:
    	lidar_enable_ar = 0;
    	lidar_enable_av = 0;
     	angle(_client, -2.36);
    	state++;
    	break;
    case 61:
      if(check_position(_client))
        state = 70;
      break;
    case 70:
      //ramasse les balles de la bonne couleur
      //do_AX12action(_client, "TRICANON\n");
      state = 80;
      break;
    case 80:
      //if(do_AX12action(_client, "OVERTRICANON\n"))
      state = 90;
      break;
    case 90:
    	lidar_enable_ar = 1;
    	move_xy(_client, 0.1, -0.13);
    	state = 100;
    	break;
    case 100:
  		//if((x_odo > 0.01 && x_odo < 0.15) && (y_odo > -0.18 && y_odo < -0.08))
      if(check_position(_client))
    		state = 110;
  	  break;
    /*case 101:
    	lidar_enable_ar = 0;
    	angle(_client, 0);
    	state++;
  	  break;
    case 102:
    	if(check_position(_client))
    		state++;
    	break;
    case 103:
    	lidar_enable_av = 1;
    	lidar_enable_ar = 0;
      move_xy(_client, 0.7, -0.13);
  	  state++;
      break;
    case 104:
     	if(check_position(_client))
    		state++;
    	break;
    case 105:
    	lidar_enable_av = 0;
    	lidar_enable_ar = 1;
    	move_xy(_client, 0.3, -0.13);
    	state = 102;
    	break;*/
    case 110:
    	lidar_enable_ar = 0;
    	angle(_client, 1.571);
    	state++;
    	break;
    case 111:
      if(check_position(_client))
        state = 120;
      break;
		case 120:
    ////////// Check Lidar
      lidar_enable_ar = 1;
      lidar_enable_av = 0;
			move_xy(_client, 0.1, 0.05);
			state = 130;
			break;
		case 130:
			//if((x_odo > 0.05 && x_odo < 0.15) && (y_odo > 0 && y_odo < 0.15))
      if(check_position(_client))
    		state = 140;
			break;
		case 140:
 			angle(_client, 1.571);
			state++;
			break;
    case 141:
      if(check_position(_client))
        state = 150;
      break;
		case 150: //envoie les balles dans la citerne
      //do_AX12action(_client, "TURBON\n");
      state = 160;
			break;
    case 160:
      // if(do_AX12action(_client, "OVERTURBON\n"))
      state = 161;
      break;
    case 161:
      // do_AX12action(_client, "OUVRIRCANON\n");
      state = 162;
      break;
    case 162:
      // if(check_AX12action(_client, "OVEROUVRIRCANON\n"))
      state = 170;
      break;
    case 170:
      // Tempo le temps de lancer les balles
      tempo++;
      if(tempo > 20)
      {
        state = 180;
        tempo = 0;
      }
      break;
    case 180:
      // do_AX12action(_client, "TURBOFF\n");
      state++;
      break;
    case 181:
      // if(do_AX12action(_client, "OVERTURBOFF\n"))
      state = 190;
      break;
    case 190:
      // do_AX12action(_client, "FERMERCANON\n");
      state++;
      break;
    case 191:
      // if(do_AX12action(_client, "OVERFERMERCANON\n"))
      state = 200;
      break;
		case 200:
      lidar_enable_ar = 0;
      lidar_enable_av = 0;
 			angle(_client, -0.786);
			state++;
			break;
    case 201:
      if(check_position(_client))
        state = 210;
      break;
		case 210:
  		lidar_enable_av = 1;
  		//lidar_enable_ar = 1;
  		move_xy(_client, 0.61, -0.56);
  		state = 220;
  		break;
		case 220:
			//if((x_odo > 0.56 && x_odo < 0.66) && (y_odo > -0.61 && y_odo < -0.51))
      if(check_position(_client))
    		state = 230;
			break;
		case 230:
      lidar_enable_ar = 0;
      lidar_enable_av = 0;
 			angle(_client, 0);
			state++;
			break;
    case 231:
      if(check_position(_client))
        state = 240;
      break;
		case 240:
      lidar_enable_av = 1;
			move_xy(_client, 2.14, -0.6);
			state = 250;
			break;
		case 250:
			//if((x_odo > 1.99 && x_odo < 2.19) && (y_odo > -0.65 && y_odo < -0.55))
      if(check_position(_client))
    		state = 260;
			break;
		case 260:
      lidar_enable_av = 0;
 			angle(_client, -0.838);
			state++;
			break;
    case 271:
      if(check_position(_client))
        state = 270;
      break;
		case 270:
      lidar_enable_av = 1;
			move_xy(_client, 2.595, -1.14);
			state = 280;
			break;
		case 280:
			//if((x_odo > 2.545 && x_odo < 2.645) && (y_odo > -1.19 && y_odo < -1.09))
      if(check_position(_client))
    		state = 290;
			break;
		case 290:
      lidar_enable_av = 0;
 			angle(_client, -2.478);
			state++;
			break;
    case 291:
      if(check_position(_client))
        state = 300;
      break;
		case 300:
			move_xy(_client, 2.34, -1.34);
			state = 310;
			break;
		case 310:
			//if((x_odo > 2.29 && x_odo < 2.39) && (y_odo > -1.39 && y_odo < -1.29))
      if(check_position(_client))
    		state = 320;
			break;
		case 320:
 			angle(_client, -2.478);
			state++;
			break;
    case 321:
      if(check_position(_client))
        state = 330;
      break;
		case 330: //ramasse les balles qui sont à trier
      // do_AX12action(_client, "TRIALT\n");
      state = 340;
      break;
    case 340:
      //if(do_AX12action(_client, "OVERTRIALT\n"))
			state = 350;
			break;
		case 350:
      ///////// Check Lidar
      lidar_enable_ar = 1;
			move_xy(_client, 2.595, -1.14);
			state = 260;
			break;
		case 36:
			//if((x_odo > 2.545 && x_odo < 2.645) && (y_odo > -1.19 && y_odo < -1.09))
      if(check_position(_client))
    		state = 370;
			break;
		case 370:
      lidar_enable_ar = 0;
 			angle(_client, -3.648);
			state++;
			break;
    case 371:
      if(check_position(_client))
        state = 380;
      break;
		case 380:
      lidar_enable_av = 1;
			move_xy(_client, 1.45, -0.52);
			state = 390;
			break;
		case 390:
			//if((x_odo > 1.4 && x_odo < 1.5) && (y_odo > -0.57 && y_odo < -0.47))
      if(check_position(_client))
    		state = 400;
			break;
		case 400:
      lidar_enable_av = 0;
 			angle(_client, 1.571);
			state++;
			break;
    case 401:
      if(check_position(_client))
        state = 410;
      break;
		case 410:
      lidar_enable_ar = 1;
			move_xy(_client, 1.45, -1.09);
			state = 420;
			break;
		case 420:
			//if((x_odo > 1.4 && x_odo < 1.5) && (y_odo > -1.14 && y_odo < -1.04))
      if(check_position(_client))
    		state = 430;
			break;
		case 430: //dépose balles triées
      //do_AX12action(_client, "OUVRIRTRAPPE\n");
      state = 440;
      break;
    case 440:
      //if(do_AX12action(_client, "OVEROUVRIRTRAPPE\n"))
      state = 450;
      break;
    case 450:
      //tempo pour que les balles tombent
			tempo++;
			if(tempo > 20)
			{
  			state = 460;
  			tempo = 0;
			}
			break;
    case 460:
      //do_AX12action(_client, "FERMERTRAPPE\n");
      state = 470;
      break;
    case 470:
      //if(do_AX12action(_client, "OVERFERMERTRAPPE\n"))
      state = 480;
      break;
		case 480:
      lidar_enable_ar = 0;
 			angle(_client, 2.443);
			state++;
			break;
    case 481:
      if(check_position(_client))
        state = 490;
      break;
		case 490:
      lidar_enable_av = 1;
			move_xy(_client, 0.085, 0);
			state = 500;
			break;
		case 500:
			//if((x_odo > 0.035 && x_odo < 0.135) && (y_odo > -0.05 && y_odo < 0.05))
      if(check_position(_client))
    		state = 510;
			break;
		case 510:
      lidar_enable_av = 0;
 			angle(_client, 1.571);
			state++;
			break;
    case 511:
      if(check_position(_client))
        state = 520;
      break;
		case 520: //Tire balles triées
      //do_AX12action(_client, "TURBON\n");
      state = 530;
      break;
    case 530:
      //if(do_AX12action(_client, "OVERTURBON\n"))
      state = 540;
      break;
    case 540:
      //do_AX12action(_client, "OUVRIRCANON\n");
      state = 550;
      break;
    case 550:
      //if(do_AX12action(_client, "OVEROUVRIRCANON\n"))
      state = 560;
      break;
    case 560:
      // Tempo le temps de lancer les balles
			tempo++;
			if(tempo > 20)
			{
  			state = 570;
  			tempo = 0;
			}
			break;
    case 570:
      //do_AX12action(_client, "TURBOFF\n");
      state = 580;
      break;
    case 580:
      //if(do_AX12action(_client, "OVERTURBOFF\n"))
      state = 0;
      break;
  }
}

string get_team(ros::ServiceClient client)
{
  string value;
  comm::Comm srv;

  srv.request.command = "GETTEAM\n";

  if(client.call(srv))
    value = (string)srv.response.answer;

  return value;
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

  string team = get_team(_client);
  if (team.find("COULEUR:0"))
    cote_orange();
  else
    cote_vert();

  //std::cout << date.sec << " " << date.nsec << " " << x_odo << " " << y_odo << " " << theta_odo
  //<< " " << state << std::endl;
}

string getStart(ros::ServiceClient client)
{
  string value;
  comm::Comm srv;

  srv.request.command = "MATCHSTATUS\n";

  if(client.call(srv))
    value = (string)srv.response.answer;

  return value;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "script_match");
  ros::NodeHandle n;

  _client = n.serviceClient<comm::Comm>("pic_pi_comm");

  ros::Subscriber sub = n.subscribe("scan", 1000, lidar_callback);

  ros::Timer timer = n.createTimer(ros::Duration(0.5), script_callback);

  ros::spin();

  return 0;
}
