#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

void callback(const PointCloud::ConstPtr& msg) // callback function for the subscriber, prcesses the input 
{
   printf ("Cloud: width = %d, height = %d\n", msg->width, msg->height);
   BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
      printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z); 
}

int main(int argc, char** argv)
{
   ros::init(argc, argv, "pcl_sub_laser_pub");  // initialize node 
   ros::NodeHandle n;
   ros::Publisher scan_pub = n.advertise<sensor_msgs::LaserScan>("scan", 50); // publishing scan 
   unsigned int num_readings = 100;
   double laser_frequency = 40;
   double ranges[num_readings];
   double intensities[num_readings];
   
   int count = 0;
   ros::Rate r(1.0);

   while(n.ok())
   {
      //generate some fake data for our laser scan
      for(unsigned int i = 0; i < num_readings; ++i)
      {
         ranges[i] = count;
         intensities[i] = 100 + count;
      }
      ros::Time scan_time = ros::Time::now();

      //populate the LaserScan message
      sensor_msgs::LaserScan scan;
      scan.header.stamp = scan_time;
      scan.header.frame_id = "body";
      scan.angle_min = -1.57;
      scan.angle_max = 1.57;
      scan.angle_increment = 3.14 / num_readings;
      scan.time_increment = (1 / laser_frequency) / (num_readings);
      scan.range_min = 0.0;
      scan.range_max = 100.0;

      scan.ranges.resize(num_readings);
      scan.intensities.resize(num_readings);

      for(unsigned int i = 0; i < num_readings; ++i)
      {
         scan.ranges[i] = ranges[i];
         scan.intensities[i] = intensities[i];
      }
      ros::Subscriber sub = nh.subscribe<PointCloud>("/local_cloud", 1, callback);
      scan_pub.publish(scan);
      ++count;
      r.sleep();
   }
}
