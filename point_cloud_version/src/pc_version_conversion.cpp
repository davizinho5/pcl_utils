/*
* Data type converter
* From sensor_msgs/PointCloud.h to sensor_msgs/PointCloud2.h
 */

#include <ros/ros.h>
#include <boost/thread.hpp>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud_conversion.h>

using namespace std;

class PointCloudConverter 
{
  private:
    boost::mutex m_mutex_;
    // ROS
    ros::NodeHandle nh_;
    ros::Subscriber sub_points_, sub_points2_;
    ros::Publisher pub_points_, pub_points2_;

    int queue_size_;
    string points_in_, points2_in_, points_out_, points2_out_;

  public:
    PointCloudConverter () : nh_ ("~"), queue_size_ (100), 
                             points_in_ ("/points_in"), points2_in_ ("/points2_in"), 
                             points_out_ ("/points_out"), points2_out_ ("/points2_out")
    {
      // Subscribe to the cloud topic using both the old message format and the new
      sub_points_ = nh_.subscribe (points_in_, queue_size_, &PointCloudConverter::cloud_cb_points, this);
      sub_points2_ = nh_.subscribe (points2_in_, queue_size_, &PointCloudConverter::cloud_cb_points2, this);
      pub_points_ = nh_.advertise<sensor_msgs::PointCloud> (points_out_, queue_size_);
      pub_points2_ = nh_.advertise<sensor_msgs::PointCloud2> (points2_out_, queue_size_);
      ROS_INFO ("PointCloudConverter initialized to transform from PointCloud (%s) to PointCloud2 (%s).", nh_.resolveName (points_in_).c_str (), nh_.resolveName (points2_out_).c_str ());
      ROS_INFO ("PointCloudConverter initialized to transform from PointCloud2 (%s) to PointCloud (%s).", nh_.resolveName (points2_in_).c_str (), nh_.resolveName (points_out_).c_str ());
    }
    

    inline std::string getFieldsList (const sensor_msgs::PointCloud2 &cloud)
    {
      std::string result;
      for (size_t i = 0; i < cloud.fields.size () - 1; ++i)
        result += cloud.fields[i].name + " ";
      result += cloud.fields[cloud.fields.size () - 1].name;
      return (result);
    }


    inline std::string getFieldsList (const sensor_msgs::PointCloud &points)
    {
      std::string result = "x y z";
      for (size_t i = 0; i < points.channels.size (); i++)
        result = result + " " + points.channels[i].name;
      return (result);
    }


    void cloud_cb_points2 (const sensor_msgs::PointCloud2ConstPtr &msg)
    {
      if (pub_points_.getNumSubscribers () <= 0)
      {
        //ROS_DEBUG ("[point_cloud_converter] Got a PointCloud2 with %d points on %s, but no subscribers.", msg->width * msg->height, nh_.resolveName (points2_in_).c_str ());
        return;
      }

      ROS_DEBUG ("[point_cloud_converter] Got a PointCloud2 with %d points (%s) on %s.", msg->width * msg->height, getFieldsList (*msg).c_str (), nh_.resolveName (points2_in_).c_str ());
      
      sensor_msgs::PointCloud output;
      // Convert to the new PointCloud format
      if (!sensor_msgs::convertPointCloud2ToPointCloud (*msg, output))
      {
        ROS_ERROR ("[point_cloud_converter] Conversion from sensor_msgs::PointCloud2 to sensor_msgs::PointCloud failed!");
        return;
      }
      ROS_DEBUG ("[point_cloud_converter] Publishing a PointCloud with %d points on %s.", (int)output.points.size (), nh_.resolveName (points_out_).c_str ());
      pub_points_.publish (output);
    }


    void cloud_cb_points (const sensor_msgs::PointCloudConstPtr &msg)
    {
      if (pub_points2_.getNumSubscribers () <= 0)
      {
        //ROS_DEBUG ("[point_cloud_converter] Got a PointCloud with %d points on %s, but no subscribers.", (int)msg->points.size (), nh_.resolveName (points_in_).c_str ());
        return;
      }

      ROS_DEBUG ("[point_cloud_converter] Got a PointCloud with %d points (%s) on %s.", (int)msg->points.size (), getFieldsList (*msg).c_str (), nh_.resolveName (points_in_).c_str ());

      sensor_msgs::PointCloud2 output;
      // Convert to the old point cloud format
      if (!sensor_msgs::convertPointCloudToPointCloud2 (*msg, output))
      {
        ROS_ERROR ("[point_cloud_converter] Conversion from sensor_msgs::PointCloud to sensor_msgs::PointCloud2 failed!");
        return;
      }
      ROS_DEBUG ("[point_cloud_converter] Publishing a PointCloud2 with %d points on %s.", output.height * output.width, nh_.resolveName (points2_out_).c_str ());
      pub_points2_.publish (output);
    }

};


int main (int argc, char** argv) {
  // ROS init
  ros::init (argc, argv, "point_cloud_converter", ros::init_options::AnonymousName);

  PointCloudConverter p;
  ros::spin ();

  return (0);
}
