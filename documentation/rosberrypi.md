# Rospberrypi Configuration

Rospberrypi c'est une distribution linux pour Raspberry Pi qui comporte déja ROS installer. 

ROS (Robot Operating System) c'est une surcouche de linux qui offre des outils open sources pour créer des robots (partie software). ROS offre en plus la possibilité de répartir ses applications sur plusieurs ordinateurs avec des outils de réseaux. Le site de ROS est [ici](https://ros.org/).

ROS met à disposition des interfaces vers deux langages ed programations:
* C++ (Recommandé)
* Python

## Télécharger et installer linux sur une carte SD

Vous trouverez l'image de la carte SD [ici](https://downloads.ubiquityrobotics.com/pi.html). Téléchargez la dernière version.

Pour extraire l'archive utilisez `unxz`. To install unxz run the following command:

```bash
$ sudo apt install xz-utils
$ unxz file.xz
```

Une fois l'archive extraite, on obtient une image de carte SD (file.img). On peut directement copier cette image sur la carte SD. 

**/!\ Cette opération écrase toutes données présentes sur la carte SD**

Tout d'abords, insérez la carte micro SD dans votre ordi avec un adaptateur. Localisez la carte dans */etc/dev*. Les noms possibles sont /dev/sdX (/dev/sdaX est surement votre disque dur alors faites attentions) ou /dev/mmcblk(0 ou 1) si votre carte SD comporte plusieurs partitions.

```bash
$ ls /dev/
```

Ensuite, vous pouvez utiliser `dd` pour copier l'image sur la carte SD:

```bash
$ sudo dd if=/path/to/image.img of=/dev/carte_sd conv=fsync
```

Une fois l'opération finis on peut passer sur la PI.

## Configuration de la PI

Pour se connecter sur la PI:
* Login : ubuntu
* Password : ubuntu

Un des premiers points est de changer le nom de la PI, le nom d'utilisateur et le mot de passe.

```bash
$ sudo echo "new_name">/etc/hostname
$ sudo /etc/init.d/hostname.sh start
```

**/!\ Ce changement peut impacter le système mais de mémoire il n'y a pas de problème**

```bash
$ passwd
...
```

Vous pouvez aussi changer le nom d'utilisateur mais cela **IMPACTERA** le système notemment au niveau de ROS.

```bash
$ sudo adduser temp
(set password)
$ sudo adduser temp sudo
```
Ensuite deconnectez vous (revenir à l'écran d'accueil d'identification) et utiliser Ctrl+Alt+F1 pour aller sur le terminal 1. Loggez vous avec l'utilisateur temp créer précédemment.
```
$ sudo usermod -l newUserName oldUserName
$ sudo usermod -d /home/newHomeDir -m newUserName
```

Une erreur peut survenir si l'utilisateur dont vous voulez changer le nom possède des processus en cour d'éxécutuion. Pour cela:
```
$ ps - e | grep OldUserName
$ kill ...
```

Utilisez Ctrl-d pour vous deconnecter avec la session temp et utilisez et Ctrl+Alt+7 pour revenir sur une session graphique. Vous devriez pouvoir vous log avec vos nouveaux identifiants.

Les fichiers tel que /usr/sbin/magni-base devraient avoir besoin d'une modification pour remplacer */home/ubuntu* par */home/NewName*

Pour supprimer l'utilisateur temp:

```bash
$ sudo deluser temp
$ sudo rm -r /home/temp
```
Vous avez maintenant votre système configurer.

### Configuration d'un HotSpot WiFi
PIFI est un module qui va charger une configuration hotspot wifi au démarrage.
Les fichiers de configuration sont dans */etc/PIFI/*.
Vous pouvez modifier le fichier *default* pour modifier le hotspot wifide base. Attention la clé wifi doit être suffisament longue.


### Configuration d'un serveur web
Doc apache2 et désinstaller nginx

### Configuration ROS
Modifiez la ligne avec `roslaunch ...` dans */usr/sbin/magni-base*

Installer PIGPIO et faire `$ sudo systemctl enable pigpiod.service`
