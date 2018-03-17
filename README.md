# Lidar
Lidar developpement repository for 7Robot

Règle du repository.

Dans documentation:
	On utilise le format markdown
	On cite les sources

Dans ROS
	On ajoute les packages ROS et les fichiers de configuration type .launch

	Package setup_lidar
		-tf_broadcaster: publie une tf. Modifier les valeurs pour adapter au robot.
		-pointcloud_publisher: publie un pointcloud à partir des données du lidar. Cette node applique la tf sur le pointcloud.

	Package comm
		-comm: réalise la communication entre ros et le pic et les autres nodes via un service. **Node à lancer en super user.**
		-testComm: test la comm. La node comm doit être lancer au préalable.

	Package odo
		-odo: Utilise le service de la node comm pour lire les données de l'odométrie et les publie dans le topic associé.

	Package robot_2dnav
		Package pour setup la navigation stack travail en cours
