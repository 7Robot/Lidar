#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/LinearMath/Matrix3x3.h>
#include <tf/LinearMath/Quaternion.h>
#include <string>
#include <iostream>
#include "comm/Comm.h"

using namespace std;

float x_odo = 0;
float y_odo = 0;
float theta_odo = 0;

ros::ServiceClient _client;
//TODO add checks for all move and angle change
//TODO add movement around cubes
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
}

bool check_AX12action(ros::ServiceClient client, string command)
{
  comm::Comm srv;
  bool ok = false;
  string str;

  srv.request.command = command;

  if(client.call(srv))
  {
    str = (string)srv.response.answer;
    cout << str << endl;
    if(str.find("STATE:0") != string::npos) // == "STATE:0")
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

/* Actions AX12 à appeler
"AX12INIT\n";
"OVERAX12INIT\n";
"RANGERBRASGAUCHE\n";
"OVERRANGERBRASGAUCHE\n";
"BRASGAUCHEABE\n";
"OVERBRASGAUCHEABE\n";
"BRASGAUCHECOL\n";
"OVERBRASGAUCHECOL\n";
"RANGERBRASDROIT\n";
"OVERRANGERBRASDROIT\n";
"BRASDROITABE\n";
"OVERBRASDROITABE\n";
"BRASDROITCOL\n";
"OVERBRASDROITCOL\n";
"FERMERPINCEAV\n";
"OVERFERMERPINCEAV\n";
"FERMERPINCEAR\n";
"OVERFERMERPINCEAR\n";
"RECUPCUBEAV\n";
"OVERRECUPCUBEAV\n";
"RECUPCUBEAR\n";
"OVERRECUPCUBEAR\n";
"RECUPDERAV\n";
"OVERRECUPDERAV\n";
"RECUPDERAR\n";
"OVERRECUPDERAR\n";
"LIBCUBEAV\n";
"OVERLIBCUBEAV\n";
"LIBCUBEAR\n";
"OVERLIBCUBEAR\n";
"INTAV\n";
"OVERINTAV\n";
"INTAR\n";
"OVERINTAR\n";
"TESTAX\n";
"OVERTESTAX\n";
*/

bool check_position(ros::ServiceClient client)
{
  // AsservDone == 0 => non terminée
  comm::Comm srv;
  bool asservDone = false;
  string str;

  srv.request.command = "DONE\n";

  if(client.call(srv))
  {
    str = (string)srv.response.answer;
    if(str.find("1") != string::npos) // == "1")
      asservDone = true;
      //cout << str << endl;
  }

  return asservDone;
}

bool move_xy(ros::ServiceClient client, float x, float y)
{
  comm::Comm srv;
  string str;
  stringstream ss;
  bool ok = false;

  //Action a réaliser sur le pic
	ss << fixed;
	ss.precision(4);
  ss << "MOVESEG " << x << " " << y << endl;
  srv.request.command = ss.str();

  if(client.call(srv))
  {
    str = (string)srv.response.answer;
    if(str == "STATE:0")
      ok = true;
  }
  return ok;
}

bool angle(ros::ServiceClient client, float theta)
{
  comm::Comm srv;
  string str;
  stringstream ss;
  bool ok = false;

  //Action a réaliser sur le pic
	ss << fixed;
	ss.precision(4);
  ss << "ANGLE " << theta << endl;
  srv.request.command = ss.str();

  if(client.call(srv))
  {
    str = (string)srv.response.answer;
    if(str == "STATE:0")
      ok = true;
  }
  return ok;

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

bool move_orangeBleu()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;
  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      move_xy(_client, x_init + 0.1, y_init);
      state++;
      break;
    case 1:
      if((x_odo > x_init - 0.05 && x_odo < 0.15) && (y_odo > y_init - 0.05 && y_odo < y_init + 0.05))
        state++;
      break;
    case 2:
      angle(_client, theta_init + 1.571);
      state++;
      break;
    case 3:
      move_xy(_client, x_init + 0.1, y_init - delta1 - 0.1 );
      state++;
      break;
    case 4:
      if((x_odo > x_init + 0.05 && x_odo < 0.15) && (y_odo > y_init - delta1 - 0.15 && y_odo < y_init - delta1 - 0.05))
        state++;
      break;
    case 5:
      angle(_client, theta_init);
      state++;
      break;
    case 6:
      move_xy(_client, x_init - delta2 , y_init - delta1 - 0.1);
      state++;
      break;
    case 7:
      if((x_odo > x_init - delta2 - 0.05 && x_odo < x_init - delta2 + 0.05) && (y_odo > y_init - delta1 - 0.15 && y_odo < y_init - delta1 - 0.05))
        state++;
      break;
    case 8:
      angle(_client, theta_init - 1.571);
      state++;
      break;
    case 9:
      move_xy(_client, x_init - delta2, y_init - delta1);
      state++;
      break;
    case 10:
      if((x_odo > x_init - delta2 - 0.05 && x_odo < x_init - delta2 + 0.05) && (y_odo > y_init - delta1 - 0.05 && y_odo < y_init - delta1 + 0.05))
        state++;
      break;
    case 11:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;

  return fin;
}

bool move_vertNoir()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      y = y_init + delta1 + 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init;
      angle(_client, theta);
      state++;
      break;
    case 6:
      x = x_init + delta2;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init - 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      y = y_init + delta1;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_bleuVert()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init;
      y = y_init - 0.1;
      move_xy(_client, x, y );
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      x = x_init - delta1 - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init;
      angle(_client, theta);
      state++;
      break;
    case 6:
      y = y_init + delta2;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init - 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      x = x_init - delta1;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_noirOrange()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init;
      y = y_init+0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      x = x_init + delta1 + 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init;
      angle(_client, theta);
      state++;
      break;
    case 6:
      y = y_init + delta2;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init - 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      x = x_init + delta1;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_orangeVert()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init + 0.1;
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      y = y_init - delta1 - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init;
      angle(_client, theta);
      state++;
      break;
    case 6:
      x = x_init - 2*delta2 - 0.1;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init - 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      theta = theta_init - 3.142;
      angle(_client, theta);
      state++;
    case 12:
      x = x_init - 2*delta2;
      move_xy(_client, x, y);
      state++;
      break;
    case 13:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 14:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_bleuNoir()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init;
      y = y_init - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      //if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
      if(check_position(_client))
        state++;
      break;
    case 2:
      theta = theta_init - 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      x = x_init + delta2 + 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      //if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
      if(check_position(_client))
        state++;
      break;
    case 5:
      theta = theta_init;
      angle(_client, theta);
      state++;
      break;
    case 6:
      y = y_init + 2*delta1 + 0.1;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      //if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
      if(check_position(_client))
        state++;
      break;
    case 8:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      x = x_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      //if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
      if(check_position(_client))
        state++;
      break;
    case 11:
        theta = theta_init + 3.142;
        angle(_client, theta);
        state++;
        break;
    case 12:
        y = y_init + 2*delta1;
        move_xy(_client, x, y);
        state++;
        break;
    case 13:
        //if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        if(check_position(_client))
          state++;
        break;
    case 14:
      //fonction attraper cube
      do_AX12action(_client, "RECUPCUBEAV\n")
      state++;
      break;
    case 15:
      if(check_AX12action(_client, "OVERRECUPDERAV\n"))
      {
        state = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_orangeJaune()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x;
  float y;
  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  move_orangeBleu();

  switch(state)
  {
    case 0:
      x = x_init - 0.54;
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
    //fonction attraper cube
    tempo++;
    if(tempo > 50)
    {
      state = 0;
      tempo = 0;
      fin = true;
    }
  break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_bleuOrange()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière
  switch(state)
  {
    case 0:
      x = x_init;
      y = y_init - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      x = x_init + delta2 - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init + 3.142;
      angle(_client, theta);
      state++;
      break;
    case 6:
      y = y_init + delta1;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      x = x_init + delta2;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_bleuJaune()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;
  float x;
  float y;
  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  move_bleuOrange();// à modifier?

  switch(state)
  {
    case 0:
      x = x_init+0.54;
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
    //fonction attraper cube
    tempo++;
    if(tempo > 50)
    {
      state = 0;
      tempo = 0;
      fin = true;
    }
  break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_jauneVert()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;
  float x;
  float y;
  switch(state)
  {
    case 0:
      x = x_init - 0.54;
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
    //fonction attraper cube
    tempo++;
    if(tempo > 50)
    {
      state = 0;
      tempo = 0;
      fin = true;
    }
  break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_jauneNoir()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;
  float x;
  float y;

  switch(state)
  {
    case 0:
      x = x_init;
      y = y_init + 0.54;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
    //fonction attraper cube
    tempo++;
    if(tempo > 50)
    {
      state = 0;
      tempo = 0;
      fin = true;
    }
  break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_noirJaune()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;
  move_noirOrange();// à modifier?
  float x;
  float y;

  switch(state)
  {
    case 0:
      x = x_init - 0.54;
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
    //fonction attraper cube
    tempo++;
    if(tempo > 50)
    {
      state = 0;
      tempo = 0;
      fin = true;
    }
  break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_vertBleu()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      y = y_init - delta2 + 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init + 3.142;
      angle(_client, theta);
      state++;
      break;
    case 6:
      x = x_init + delta1;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      y = y_init - delta2;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_noirVert()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init;
      y = y_init + 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      x = x_init - delta2 - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init + 3.142;
      angle(_client, theta);
      state++;
      break;
    case 6:
      y = y_init - delta1;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      x = x_init - delta2;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_vertOrange()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init - 0.1;
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init - 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      y = y_init - delta1 - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init;
      angle(_client, theta);
      state++;
      break;
    case 6:
      x = x_init + 2*delta2 + 0.1;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
        theta = theta_init + 3.142;
        angle(_client, theta);
        state++;
        break;
    case 12:
        x = x_init + 2*delta2;
        move_xy(_client, x, y);
        state++;
        break;
    case 13:
        if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
          state++;
        break;
    case 14:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_noirBleu()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init;
      y = y_init + 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init - 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      x = x_init + delta2 + 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init - 3.142;
      angle(_client, theta);
      state++;
      break;
    case 6:
      y = y_init - 2*delta1 - 0.2;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init - 4.713;
      angle(_client, theta);
      state++;
      break;
    case 9:
      x = x_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      theta = theta_init - 3.142;
      angle(_client, theta);
      state++;
      break;
    case 12:
      y = y_init - 2*delta1 - 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 13:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 14:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_orangeNoir()
{
  static int state = 0;
  static int tempo = 0;
  bool fin = false;

  float x_init = x_odo;// à changer en fonction de l'emplacement où nous sommes
  float y_init = y_odo;
  float theta_init = 0;

  float x = 0;
  float y = 0;
  float theta = 0;

  float delta1 = 22; // à changer si avant ou arrière
  float delta2 = 18.3; // à changer si avant ou arrière

  switch(state)
  {
    case 0:
      x = x_init + 0.1;
      y = y_init;
      move_xy(_client, x, y);
      state++;
      break;
    case 1:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 2:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 3:
      y = y_init - delta2 + 0.1;
      move_xy(_client, x, y);
      state++;
      break;
    case 4:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 5:
      theta = theta_init + 3.142;
      angle(_client, theta);
      state++;
      break;
    case 6:
      x = x_init - delta1;
      move_xy(_client, x , y);
      state++;
      break;
    case 7:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 8:
      theta = theta_init + 1.571;
      angle(_client, theta);
      state++;
      break;
    case 9:
      y = y_init - delta1;
      move_xy(_client, x, y);
      state++;
      break;
    case 10:
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
        state++;
      break;
    case 11:
      //fonction attraper cube
      tempo++;
      if(tempo > 50)
      {
        state = 0;
        tempo = 0;
        fin = true;
      }
    break;
  }
  std::cout << state << std::endl;
  return fin;
}

bool move_randomSpotC()
{
  static int state = 0;
  bool fin = false;

  switch (state) {
    case 0:
      if(move_bleuOrange())
        state++;
      break;
    case 1:
      if(move_orangeNoir())
        state++;
      break;
    case 2:
      if(move_noirJaune())
        state = 0;
        fin = true;
      break;
  }
}

void move_seq1SpotC()
{
  static int state = 0;

  switch (state) {
    case 0:
      if(move_noirOrange())
        state++;
      break;
    case 1:
      if(move_orangeBleu())
        state++;
      break;
    case 2:
      if(move_bleuJaune())
        state = 0;
      break;
  }
}

void move_seq2SpotC()
{
  static int state = 0;

  switch (state) {
    case 0:
      if(move_noirBleu())
        state++;
      break;
    case 1:
      if(move_bleuOrange())
        state++;
      break;
    case 2:
      if(move_orangeJaune())
        state = 0;
      break;
  }
}

void move_seq7SpotC()
{
  // avoir pensé à poser le Golden cube pendant le match
  static int state = 0;

  switch (state) {
    case 0:
      if(move_bleuOrange())
        state++;
      break;
    case 1:
      if(move_orangeNoir())
        state++;
      break;
    case 2:
      if(move_noirJaune())
        state = 0;
      break;
  }
}

void move_seq9SpotC()
{
  static int state = 0;

  switch (state) {
    case 0:
      if(move_bleuNoir())
        state++;
      break;
    case 1:
      if(move_noirOrange())
        state++;
      break;
    case 2:
      if(move_orangeJaune())
        state = 0;
      break;
  }
}

void script_callback(const ros::TimerEvent& trash)
{
  static int state = 0;
  static int tempo = 0;
  //float x_odo = 0;
  //float y_odo = 0;
  //float theta_odo = 0;
  switch(state)
  {
    //Depart
    case 0:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 0.885, 0);
      state++;
      break;
    //Devant l'interrupteur
    case 1:
      //if((x_odo > 0.83 && x_odo < 0.93) && (y_odo > -0.05 && y_odo < 0.05))
      if(check_position(_client))
        state = 10;
      break;
    case 10:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client, 1.571);
      state = 20;
      break;
    case 20:
      // Check angle
      if(check_position(_client))
        state = 30;
      break;
    case 30:
      //activation interrupteur
      do_AX12action(_client, "INTAV\n");
      state = 40;
      break;
    case 40:
      //check activation fini
      if(check_AX12action(_client, "OVERINTAV\n"))
        state = 50;
      break;
    //Recul
    case 50:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client, 1.833);
      state = 60;
      break;
    case 60:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 1.04, -0.55);
      state = 70;
      break;
    case 70:
      //(x_odo > 0.99 && x_odo < 1.09) && (y_odo > -0.6 && y_odo < -0.5)
      if(check_position(_client))
        state = 80;
      break;
    case 80:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client, -2.315);
      state = 90;
      break;
    case 90:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 0.0, -1.52);
      state = 100;
      break;
    case 100:
      //if((x_odo < 0.05 && x_odo > -0.05) && (y_odo > -1.57 && y_odo < -1.47))
      if(check_position(_client))
        state = 110;
      break;
    case 110:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client, 0);
      state = 120;
      break;
    case 120:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, -0.06, -1.52);
      state = 130;
      break;
    case 130:
      //if((x_odo < -0.01 && x_odo > -0.11) && (y_odo > -1.57 && y_odo < -1.47))
      if(check_position(_client))
        state = 140;
      break;
    case 140:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client,0);
      state = 150;
      break;
    case 150:
      //activation bras droit abeille
      do_AX12action(_client, "BRASDROITABE\n");
      state++;
      break;
    case 151:
      if(check_AX12action(_client, "OVERBRASDROITABE\n"))
        state = 160;
      break;
    case 160:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 0.17, -1.57);
      state = 170;
      break;
    case 170:
      //if((x_odo < 0.22 && x_odo > 0.14) && (y_odo > -1.63 && y_odo < -1.53))
      if(check_position(_client))
        state = 180;
      break;
    case 180:
      //remonter bras droit abeille
      do_AX12action(_client, "RANGERBRASDROIT\n");
      state++;
      break;
    case 181:
      if(check_AX12action(_client, "OVERRANGERBRASDROIT\n"))
        state = 190;
      break;
    case 190:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client, 0.96);
      state = 200;
      break;
    case 200:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 0.64, -0.77);
      state = 210;
      break;
    case 210:
      //if((x_odo < 0.69 && x_odo > 0.59) && (y_odo > -0.82 && y_odo < -0.72))
      if(check_position(_client))
        state = 220;
      break;
    case 220:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client, 1.571);
      state = 230;
      break;
    case 230:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 0.64, -0.5);
      state = 240;
      break;
    case 240:
      //if((x_odo < 0.69 && x_odo > 0.59) && (y_odo > -0.45 && y_odo < -0.55))
      if(check_position(_client))
        state = 250;
      break;
    case 250:
      //prise cubes pince AVANT
      do_AX12action(_client, "RECUPCUBEAV\n");
      state++;
      break;
    case 251:
      if(check_AX12action(_client, "OVERRECUPCUBEAV\n"))
        state++;
      break;
    case 252:
      // pince avant
      if(move_bleuNoir())
        state = 260;
      break;
    case 260:
      // Depart Noir -> Go to depot
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 0.6, -0.03);
      state = 270;
      break;
    case 270:
      //if((x_odo < 0.65 && x_odo > 0.55) && (y_odo > -0.08 && y_odo < 0.02))
      if(check_position(_client))
        state = 280;
      break;
    case 280:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client, 1.571);
      state = 290;
      break;
    case 290:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 0.6, -0.09);
      state = 300;
      break;
    case 300:
      //if((x_odo < 0.65 && x_odo > 0.55) && (y_odo > -0.14 && y_odo < -0.04))
      if(check_position(_client))
        state = 310;
      break;
    case 310:
      //ouvrir porte (depot cubes)
      do_AX12action(_client, "LIBCUBEAV\n");
      state++;
      break;
    case 311:
      //check fin ouverture
      if(do_AX12action(_client, "OVERLIBCUBEAV\n"));
        state = 320;
      break;
    case 320:
      // Reculer pour fermer
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client, 0.6, -0.2);
      state = 330;
      break;
    case 330:
      //if((x_odo < 0.65 && x_odo > 0.55) && (y_odo > -0.25 && y_odo < -0.15))
      if(check_position(_client))
        state = 340;
      break;
    case 340:
      //fermer porte
      //do_AX12action(_client, "FERMERPORTEAV\n");
      //state++;
      tempo++;
      if(tempo > 20)
      {
        state = 350;
        tempo = 0;
      }
      break;
    case 341:
      //check fin fermeture porte
      //if(check_AX12action(_client, "OVERFERMERPORTEAV\n"))
      //  state = 350;
      //break;
    case 350:
      // Go to next cubes
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client,-1.95);
      state = 360;
      break;
    case 360:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client,0.38,-0.95);
      state = 370;
      break;
    case 370:
      //if((x_odo < 0.43 && x_odo > 0.33) && (y_odo > -1.0 && y_odo < -0.90))
      if(check_position(_client))
        state = 380;
      break;
    case 380:
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client,-3.14);
      state = 390;
      break;
    case 390:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client,0.31,-0.95);
      state = 400;
      break;
    case 400:
      //if((x_odo > 0.26 && x_odo < 0.36) && (y_odo > -1.0 && y_odo < -0.90))
      if(check_position(_client))
        state = 410;
      break;
    case 410: //Prise des cubes
      // On arrive au orange
      // pince arrière
      do_AX12action(_client, "RECUPCUBEAR\n");
      state++;
      break;
    case 411:
      if(check_AX12action(_client, "OVERRECUPCUBEAR\n"))
        state = 430;
      break;
    case 430:
      // repart du orange
      get_odo(_client, x_odo, y_odo, theta_odo);
      angle(_client,1.57);
      state = 440;
      break;
    case 440:
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client,0.31,-0.03);
      state = 450;
      break;
    case 450:
      //if((x_odo > 0.26 && x_odo < 0.36) && (y_odo > -0.08 && y_odo < 0.0))
      if(check_position(_client))
        state = 460;
      break;
    case 460:
      //Ouvrir porte (déposer les cubes pince arrière)
      do_AX12action(_client, "LIBCUBEAR\n");
      state++;
      break;
    case 461:
      //check porte arrière ouverte
      if(check_AX12action(_client, "OVERLIBCUBEAR\n"))
        state = 470;
      break;
    case 470:
      // Avancer pour fermer porte arrière
      get_odo(_client, x_odo, y_odo, theta_odo);
      move_xy(_client,0.31,-0.2);
      state = 480;
      break;
    case 480:
      //if((x_odo > 0.26 && x_odo < 0.36) && (y_odo > -0.25 && y_odo < -0.15))
      if(check_position(_client))
        state = 490;
      break;
    case 490:
      //Fermer porte
      //do_AX12action(_client, "FERMERPORTEAR\n");
      //state++;
      //break;
    case 491:
      //check porte fermée
      if(check_AX12action(_client, "OVERFERMERPORTEAR\n"))
        state = 0;
      break;
  }
  std::cout << state << std::endl;
}

/*void script_callback(const ros::TimerEvent& trash)
{
  static int state = 0;
  static int tempo = 0;

  switch(state)
  {
    //Depart
    case 0:
      move_xy(_client, 0.885, 0);
      state++;
      break;
    //Devant l'interrupteur
    case 1:
      if((x_odo > 0.83 && x_odo < 0.93) && (y_odo > -0.1 && y_odo < 0.1))
        state++;
      break;
    case 2:
      angle(_client, 1.571);
      state++;
      break;
    case 3:
      tempo++;
      if(tempo > 20)
      {
        state++;
        tempo = 0;
      }
      break;
    case 4:
      //check activation fini
      state++;
      break;
    //Recul
    case 5:
      move_xy(_client, 0.885, -0.03);
      state++;
      break;
    case 6:
      if((x_odo > 0.83 && x_odo < 0.93) && (y_odo > -0.1 && y_odo < 0))
        state++;
      break;
    case 7:
      angle(_client, 0);
      state++;
      break;
    case 8:
      move_xy(_client, 1.26, -0.03);
      state++;
      break;
    case 9:
      if((x_odo < 1.31 && x_odo > 1.21) && (y_odo > -0.1 && y_odo < 0))
        state++;
      break;
    case 10:
      angle(_client, 0);
      state++;
      break;
    case 11: //Lecture capteur 1
      tempo++;
      if(tempo > 20)
      {
        state++;
        tempo = 0;
      }
      break;
    case 12:
      move_xy(_client, 1.34, -0.03);
      state++;
      break;
    case 13:
      if((x_odo < 1.39 && x_odo > 1.29) && (y_odo > -0.1 && y_odo < 0))
        state++;
      break;
    case 14:
        angle(_client, 0);
        state++;
        break;
    case 15: //Lecture capteur 2
      tempo++;
      if(tempo > 20)
      {
        state++;
        tempo = 0;
      }
      break;
    case 16:
      move_xy(_client, 1.41, -0.03);
      state++;
      break;
    case 17:
      if((x_odo < 1.46 && x_odo > 1.36) && (y_odo > -0.1 && y_odo < 0))
        state++;
      break;
    case 18:
        angle(_client, 0);
        state++;
        break;
    case 19: //Lecture capteur 3
      tempo++;
      if(tempo > 20)
      {
        state++;
        tempo = 0;
      }
      break;
    //ATTENTION!!
    case 20:
      angle(_client,0.916);
      state++;
      break;
    case 21:
      move_xy(_client,0.18,-1.57);
      state++;
      break;
    case 22:
      if((x_odo > 0.13 && x_odo < 0.23) && (y_odo > -1.62 && y_odo < -1.52))
        state++;
      break;
    case 23:
      angle(_client, 0);
      state++;
      break;
    case 24:
      move_xy(_client,0,-1.57);
      state++;
      break;
    case 25:
      if((x_odo > 0.05 && x_odo < -0.05) && (y_odo > -1.62 && y_odo < -1.52))
        state++;
      break;
    case 26:
      tempo++;
      if(tempo > 20)
      {
        state++;
        tempo = 0;
      }
      break;
    case 27:
      move_xy(_client,0.18,-1.57);
      state++;
      break;
    case 28:
      if((x_odo > 0.13 && x_odo < 0.23) && (y_odo > -1.62 && y_odo < -1.52))
        state++;
      break;
    case 29:
      angle(_client,1.05);
      state++;
      break;
    case 30:
      move_xy(_client,0.64,-0.6);
      state++;
      break;
    case 31:
      if((x_odo < 0.69 && x_odo > 0.59) && (y_odo > -0.65 && y_odo < -0.55))
        state++;
      break;
    case 32:
      angle(_client,1.571);
      state++;
      break;
    case 33:
      move_xy(_client,0.64,-0.5);
      state++;
      break;
    case 34:
      if((x_odo < 0.69 && x_odo > 0.59) && (y_odo > -0.55 && y_odo < -0.45))
        state++;
      break;
    case 35: //Prise des cubes
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    /*
    case 32:
      move_xy(_client,0.60,-0.03);
      state++;
      break;
    case 33:
      if((x_odo < 0.65 && x_odo > 0.55) && (y_odo > -0.08 && y_odo < 0.0))
        state++;
      break;
    case 34:
      angle(_client,1.571);
      state++;
      break;
    case 35:
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 36:
      move_xy(_client,0.60,-0.2);
      state++;
      break;
    case 37:
      if((x_odo < 0.65 && x_odo > 0.55) && (y_odo < -0.15 && y_odo > -0.25))
        state++;
      break;
    case 38:
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 39:
      angle(_client,-1.95);
      state++;
      break;
    case 40:
      move_xy(_client,0.38,-0.95);
      state++;
      break;
    case 41:
      if((x_odo < 0.43 && x_odo > 0.33) && (y_odo > -1.0 && y_odo < -0.90))
        state++;
      break;
    case 42:
      angle(_client,-3,14);
      state++;
      break;
    case 43:
      move_xy(_client,0.31,-0.95);
      state++;
      break;
    case 44:
      if((x_odo > 0.26 && x_odo < 0.36) && (y_odo > -1.0 && y_odo < -0.90))
        state++;
      break;
    case 45: //Prise des cubes
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 46:
      angle(_client,1.57);
      state++;
      break;
    case 47:
      move_xy(_client,0.31,-0.03);
      state++;
      break;
    case 48:
      if((x_odo > 0.26 && x_odo < 0.36) && (y_odo > -0.08 && y_odo < 0.0))
        state++;
      break;
    case 49: //Ouvrir porte (déposer les cubes)
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 50:
      move_xy(_client,0.31,-0.2);
      state++;
      break;
    case 51:
      if((x_odo > 0.26 && x_odo < 0.36) && (y_odo > -0.25 && y_odo < -0.15))
        state++;
      break;
    case 52: //Fermer porte
      tempo++;
      if(tempo > 40)
      {
        state = 0;
        tempo = 0;
      }
      break;
    */
  /*}
  std::cout << state << std::endl;
}
*/


int main(int argc, char** argv)
{
  string start = getStart(_client);
  while (start != "1") {
    start = getStart(_client);
  }

  ros::init(argc, argv, "script_match");
  ros::NodeHandle n;

  _client = n.serviceClient<comm::Comm>("pic_pi_comm");
  //ros::Subscriber sub = n.subscribe("odom", 1000, odo_callback);
  //do_AX12action(_client, "AX12INIT\n");

  //ros::Rate r(5);
  //if (do_AX12action(_client, "OVERAX12INIT\n"))
  ros::Timer timer = n.createTimer(ros::Duration(2), script_callback);

  ros::spin();

  return 0;
}
