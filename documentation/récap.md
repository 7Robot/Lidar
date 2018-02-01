#Récapitulatif de ce qu'il faut faire

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

Pour le moment, je suis allé jusqu'à l'étape 1.3.
Maintenant, il faut s'occuper de l'odométrie. Je pense que la doc pour est dans le lien précédent.
Il faut,
  -Faire une node qui permet de communiquer entre le système et la carte topics
    Une pour le PC pour le dev
    Une pour la raspi pour la coupe.
  Pour cela, je sais pas trop sur quoi partir. Pour la partie de comment fonctionne la communication, Thibaut pourra vous renseigner. Après, il y a plusieurs moyen de récupérer ces infos sur l'ordi. A vous de voir.
  Cette node pourra ensuite, transmettre les différentes informations au reste du système via des messages (topics)
  On se servira donc de cette node pour transmettre les info propre à l'odométrie.
  L'objectif de n'avoir qu'une seul node qui communique est de pouvoir facilement identifier les futurs problèmes.

  -Faire une node qui récupère les info de l'odométrie depuis la node précédente, puis les publie au bon format comme indiqué dans le tuto.

Une fois que ce sera fait, je n'ai plus de plan. Il vous faudra alors être autonome.

Vous pouvez venir me poser des questions, je vous répondrais peut être.
Si vous pensez que vous êtes perdu, ou que vous comprenez pas, RTFM. J'ai pas la science infuse et c'est comme ça que je fonctionne.
Enfin, J'espère que vous serez capable de faire quelque chose sans moi. Personne n'est indispensable ni irremplaçable.
J'ai besoin de prendre du temps pour moi. Je vais donc faire un break. Je sais pas quand je recommencerais à travailler avec vous.
