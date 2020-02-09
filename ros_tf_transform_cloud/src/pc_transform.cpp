#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_ros/transforms.h>

//#include <sensor_msgs/point_cloud_conversion.h>

//#include <pcl/point_types.h>
//#include <pcl/conversions.h>
//#include <pcl_conversions/pcl_conversions.h>
//#include <pcl/point_cloud.h>

//#include <std_msgs/Float64.h>


class PCTrasnform 
{
  private:
    // ROS
    ros::NodeHandle nh_;
    ros::Subscriber sub_pc_;
    ros::Publisher pub_trans_pc_;

  public:

PCTrasnform () 
{
  sub_pc_ = nh_.subscribe ("/cloud_pcd", 10, &PCTrasnform::pcCb, this);
  pub_trans_pc_ = nh_.advertise<sensor_msgs::PointCloud2> ("/transformedCloud", 10);
}

/*--------------------------------------------------------------------
 * Destructor
 *------------------------------------------------------------------*/
~PCTrasnform()
{ } 

void pcCb(const sensor_msgs::PointCloud2Ptr& msg) {

  tf::TransformListener tf_listener;
  tf::StampedTransform transform;
  sensor_msgs::PointCloud2 out;

  tf_listener.waitForTransform("/base_link", msg->header.frame_id, ros::Time(0), ros::Duration(1.0));
  tf_listener.lookupTransform ("/base_link", msg->header.frame_id, ros::Time(0), transform);
  pcl_ros::transformPointCloud("/base_link", transform, *msg, out);

  out.header.frame_id = "/base_link";
  pub_trans_pc_.publish(out);

}

};

int main (int argc, char** argv) {

   // Initialize ROS
  ros::init (argc, argv, "pc_transformation");

  PCTrasnform trasformer;

  ros::spin();

  return (0);
}






