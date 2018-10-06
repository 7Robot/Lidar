#include <ros/ros.h>
#include <laser_geometry/laser_geometry.h>


void lidar_callback(const sensor_msgs::LaserScan::ConstPtr& scan_in)
{
	/**
	 * scan_in fields
	 * scan_in->angle_min : start angle [rad]
	 * scan_in->angle_max : stop angle [rad]
	 * scan_in->angle_increment : step between to point [rad]
	 * scan_in->time : time between last measures
	 * scan_in->range[] : range data [m]
	 */
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "lidar_dev");
	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("base_scan", 1000, lidar_callback);

	ros::spin();

	return 0;
}
