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
      move_xy(_client, 0.885, 0);
      state++;
      break;
    case 1:
      if((x_odo > 0.83 && x_odo < 0.93) && (y_odo > -0.1 && y_odo < 0.1))
        state++;
      break;
    case 2:
      angle(_client, 1.571);
      state++;
      break;
    case 3: //Activation interrupteur
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 4:
      move_xy(_client, 0.885, -0.03);
      state++;
      break;
    case 5:
      if((x_odo > 0.83 && x_odo < 0.93) && (y_odo > -0.1 && y_odo < 0))
        state++;
      break;
    case 6:
      angle(_client, 0);
      state++;
      break;
    case 7:
      move_xy(_client, 1.26, -0.03);
      state++;
      break;
    case 8:
      if((x_odo < 1.31 && x_odo > 1.21) && (y_odo > -0.1 && y_odo < 0))
        state++;
      break;
    case 9: //Lecture capteur 1
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 10:
      move_xy(_client, 1.34, -0.03);
      state++;
      break;
    case 11:
      if((x_odo < 1.39 && x_odo > 1.29) && (y_odo > -0.1 && y_odo < 0))
        state++;
      break;
    case 12: //Lecture capteur 2
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 13:
      move_xy(_client, 1.41, -0.03);
      state++;
      break;
    case 14:
      if((x_odo < 1.46 && x_odo > 1.36) && (y_odo > -0.1 && y_odo < 0))
        state++;
      break;
    case 15: //Lecture capteur 3
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 16:
      angle(_client,0.916);
      state++;
      break;
    case 17:
      move_xy(_client,0.18,-1.57);
      state++;
      break;
    case 18:
      if((x_odo > 0.13 && x_odo < 0.23) && (y_odo > -1.62 && y_odo < -1.52))
        state++;
      break;
    case 19:
      angle(_client, 0);
      state++;
      break;
    case 20:
      move_xy(_client,0,-1.57);
      state++;
      break;
    case 21:
      if((x_odo > 0.05 && x_odo < -0.05) && (y_odo > -1.62 && y_odo < -1.52))
        state++;
      break;
    case 22:
      tempo++;
      if(tempo > 40)
      {
        state++;
        tempo = 0;
      }
      break;
    case 23:
      move_xy(_client,0.18,-1.57);
      state++;
      break;
    case 24:
      if((x_odo > 0.13 && x_odo < 0.23) && (y_odo > -1.62 && y_odo < -1.52))
        state++;
      break;
    case 25:
      angle(_client,1.05);
      state++;
      break;
    case 26:
      move_xy(_client,0.64,-0.6);
      state++;
      break;
    case 27:
      if((x_odo < 0.69 && x_odo > 0.59) && (y_odo > -0.65 && y_odo < -0.55))
        state++;
      break;
    case 28:
      angle(_client,1.571);
      state++;
      break;
    case 29:
      move_xy(_client,0.64,-0.5);
      state++;
      break;
    case 30:
      if((x_odo < 0.69 && x_odo > 0.59) && (y_odo > -0.55 && y_odo < -0.45))
        state++;
      break;
    case 31: //Prise des cubes
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
      if((x_odo > 0.65 && x_odo < 0.55) && (y_odo > -0.08 && y_odo < 0.0))
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
      if((x_odo > 0.65 && x_odo < 0.55) && (y_odo > -0.15 && y_odo < -0.25))
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
      if((x_odo > 0.43 && x_odo < 0.33) && (y_odo > -1.0 && y_odo < -0.90))
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
        state++;
        tempo = 0;
      }
      break;
    */
  }
  std::cout << state << std::endl;
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
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
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
      if((x_odo > x - 0.05 && x_odo < x + 0.05) && (y_odo > y - 0.05 && y_odo < y + 0.05))
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

void move_randomSpotC()
{
  move_orangeBleu();
  move_orangeNoir();
  move_noirJaune();
}

void move_seq1SpotC()
{
  move_noirOrange();
  move_orangeBleu();
  move_bleuJaune();
}

void move_seq2SpotC()
{
  move_noirBleu();
  move_bleuOrange();
  move_orangeJaune();
}

void move_seq7SpotC()
{
  // avoir pensé à poser le Golden cube pendant le match
  move_bleuOrange();
  move_orangeNoir();
  move_noirJaune();
  move_jauneVert();
}

void move_seq9SpotC()
{
  move_bleuNoir();
  move_noirOrange();
  move_orangeJaune();
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
