#include<ros/ros.h>
#include<std_msgs/Bool.h>
#include<std_msgs/String.h>

std_msgs::Bool teensyMsg;
ros::Publisher teensyPub;

void subToPub( const std_msgs::String::ConstPtr &charMsg ){
    std::string content = charMsg->data;
    if(content == "x")
        teensyMsg.data = true;
    else if(content == "y")
        teensyMsg.data = false;
    else
        return;
    teensyPub.publish(teensyMsg);
    return;
}




int main(int argc, char** argv){
    ros::init(argc, argv, "debug_node");

    ros::NodeHandle nh_;

    ros::Subscriber sub = nh_.subscribe("main/chatter", 1000, &subToPub);

    teensyPub = nh_.advertise<std_msgs::Bool>("teensy/chatter", 1000);

    ros::spin();

    return 0;

}