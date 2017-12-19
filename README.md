# Lidar
Lidar developpement repository for 7Robot

Règle du repository.

Dans documentation:
	On utilise le format markdown
	On cite les sources
	On fait un fichier par sujet
	..

Dans ROS
	On ajoute les packages ROS et les fichiers de configuration type .launch
	
	Package setup_lidar
		-tf_broadcaster: publie une tf. Modifier les valeurs pour adapter au robot.
		-pointcloud_publisher: publie un pointcloud à partir des données du lidar. Cette node applique la tf sur le pointcloud.
