#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

double pose_x;
double pose_y;
void callback(const turtlesim::Pose::ConstPtr& pose_data)
{
    printf("pose(%.2f, %.2f)\n", pose_data->x, pose_data->y);
    pose_x = pose_data->x;
    pose_y = pose_data->y;
    return;
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"move_control");
    ros::NodeHandle nh;
    ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel_modify",1);
    ros::Subscriber pose_sub = nh.subscribe("/turtle1/pose",10,callback);
    ros::Rate loop_rate(1);
    geometry_msgs::Twist vel_msg;
    double vel;
    double des_x;
    double des_y;
    
    while(ros::ok())
    {
        nh.getParam("/destination_x", des_x);
        nh.getParam("/destination_y", des_y);
        nh.getParam("/velocity", vel);
        // ROS_INFO("%.2f, %.2f, %.2f\n", des_x, des_y, vel);

        ros::spinOnce();
        if (pose_x <= des_x - 0.5) vel_msg.linear.x = vel;
        else vel_msg.linear.x = 0;
        if (pose_y >= des_y + 0.5) vel_msg.linear.y = -vel;
        else vel_msg.linear.y = 0;
        vel_pub.publish(vel_msg);

        loop_rate.sleep();
    }
    return 0;
}