# Navigation stack

## Connect the lidar
```
sudo ifconfig [dev] -pointopoint 169.254.157.15
```

## doc officielle: http://wiki.ros.org/navigation

Setup:
	Global:
		http://wiki.ros.org/navigation/Tutorials/RobotSetup
	Transform laser_scan into cloudpoint:
		http://wiki.ros.org/laser_pipeline/Tutorials/IntroductionToWorkingWithLaserScannerData
	Odometry
		http://wiki.ros.org/navigation/Tutorials/RobotSetup/Odom

Odo uart install serial :
```
sudo apt-get install ros-kinetic-serial
```
