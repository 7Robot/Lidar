# Setup

## Setup robot
1) Alimenter la carte pic: +16V sur Vin
2) Alimenter le lidar: connecteur Lidar sur +10V
3) Connecter le port ethernet du lidar sur le PC/RPI
  faire `sudo ifconfig [dev] -pointopoint 169.254.157.15`
  TODO faire un script pour la connexion auto du Lidar (Camille?)
4) Si on travaille avec le PC connecté le FTDI sur le port USB (cable mini USB)
  Voir document serial.md pour pouvoir utiliser le FTDI avec n'importe quel user
  TEST: faire `screen /dev/ttyUSB0 57600`
  taper une commande puis faire `ctrl + enter` jusqu'à ce qu'un message apparaisse
  Sinon reflasher le PIC.
5) Lancer les différentes nodes ROS

## Setup du code pour la compilation
Je sais pas trop mais il faut commenter tout ce qui en rapport avec le service de comm 
