# pcl_utils 
pcl_utils metapackage that contains PCL based or PCL\_ROS based (indigo) helpers and examples, which include:

__**1. cloud\_io:**__

read\_pcd\_pcl: reading a cloud saved as a .pcd file and constantly publishes it by a ROS topic as a PCL type (PointXYZRGB).


read\_pcd\_pcl: reading a cloud saved as a .pcd file and constantly publishes it by a ROS topic as a ROS type (PointCloud2).


read\_multi\_pcd\: multi-pcd file reader. Execute a loop that reads PDC files with increasing naming and publishes it by a ROS topic.


png2image: reads an image in sensor_msgs::Image format using OpenCV reader and converter. then publishes it in a ROS topic. Also publishes a topic with default calibration parameters for Kinect. 


save\_pcd: listens to a ROS topic (PointCloud2) and saves the information in .pcd files with increasing naming.


camera\_kinect\_info\_pub: publishes a ROS topic with default calibration parameters for Kinect as sensor_msgs::CameraInfo. 


camera\_xtion\_info\_pub: publishes a ROS topic with default calibration parameters for Xtion as sensor_msgs::CameraInfo. 


__**2. pcl\_euc\_seg:**__

Contains 2 examples of euclidean segmentation of objects using the PCL library. Two different launch files are provided, one of which also performs an octomap computing. Besides, examples PCDs, bag files and Rviz config files are included.

__**3. ros\_trandform\_cloud:**__

Contains one example of spatial transformation of a point cloud using the TF package from ROS. For testing, a point cloud publisher must be working too, anf the TF tree has to be setup. An example launch file with all the necessary items is placed in the launch folder. Note that, while showing the point cloud in rviz, becasue the transformation to the fixed frame is done internally, both clouds overlap in the same position. 

