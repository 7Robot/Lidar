# Récapitulatif de ce qu'il faut faire

L'objectif de ce repository est de créer un sysème ROS qui permettent d'utiliser le lidar pour la coupe de france de robotique.
Le lidar est un radar laser. Il collecte un nuage de point qu'il transmet ensuite via un bus ethernet.
Pour la configuration du Lidar, il faut demander à Thibaut Boissin.

Afin de faciliter l'intégration du lidar dans le système, nous utilisons ROS qui est une surcouche de linux.
ROS permet de faire tourner plusieurs programme ("node") qui vont pouvoir communiquer entre elles via des "topics".
ROS est plutôt fait pour tourner sur ubuntu. Je vous conseil donc de l'installer sur une machine avec ubuntu.
Pour que tout le monde soit d'accord, on va travailler avec ROS kinetic.

Avant de commencer à travailler, je vous invite à créer une machine virtuelle avec ubuntu et d'installer ROS.
J'espère que vous avez un bon PC et penser à laisser un espace disque assez grand pour que ROS puisse tourner.
Genre 15Go.
Les tutorials ROS sont très bien fait (sur le site officielle). Pour la machine virtuelle, google est votre ami.
Si vous aimez pas linux... Il va falloir, c'est beaucoup trop bien.
Enfin, il est possible d'utiliser soit python soit C++ avec l'API ROS. je vous invite à faire du C++.

Maintenant que les bases sont posées, pour le moment (01/02/2018), je (Crazer) suis parti pour utiliser le module navigation stack.
Le détail de ce sur quoi je suis parti est là:
http://wiki.ros.org/navigation/Tutorials/RobotSetup/

Pour le moment, je suis allé jusqu'à l'étape 1.4.
Maintenant, il faut s'occuper de la partie 2. Je pense que la doc est dans le lien précédent.
Il faut,
  -Faire une la costmap

  -Faire un controller pour piloter le robot

Vous pouvez venir me poser des questions, je vous répondrais peut être.
Si vous pensez que vous êtes perdu, ou que vous comprenez pas, RTFM.
