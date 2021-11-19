#include <cstdio>
#include "tf/transform_listener.h"
#include "ros/ros.h"
#include <fstream>
#include <string>
#include <iostream>

#define _USE_MATH_DEFINES
class echoListener
{
public:

    tf::TransformListener tf;

    //constructor with name
    echoListener()
    {

    }

    ~echoListener()
    {

    }

private:

};


int main(int argc, char ** argv)
{
    //Initialize ROS
    ros::init(argc, argv, "tf_echo", ros::init_options::AnonymousName);
    std::stringstream ss, ss_aux, ss_aux_raw;
    std::string append_str = "],[-2,-4,-1,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];",
                prefix_str = "rosrun tf static_transform_publisher ",
                append_str_2 = " robot_base tool0 100";
    std::ofstream outfile1("abb_targets.txt");
    std::ofstream outfile2("aux_cmds.txt");
    std::ofstream outfile3("aux_cmds_raw.txt");



    if (argc != 4)
    {
        printf("Usage: base_frame target_frame_base_name total_number_of_poses \n");
        printf(" The algorithm will find all target_frame_base_name_#number (less than total_number_of_poses) related to base_frame\n");
        return -1;
    }

    ros::NodeHandle nh("~");

    int total_number_of_poses = atof(argv[3]);

    ros::Rate rate(5);

    int precision(8);
    if (nh.getParam("precision", precision))
    {
        if (precision < 1)
        {
            std::cerr << "Precision must be > 0\n";
            return -1;
        }
        printf("Precision default value was overriden, new value: %d\n", precision);
    }

    //Instantiate a local listener
    echoListener echoListener;


    for (int current_pose = 0; current_pose < total_number_of_poses; ++current_pose) {
        std::string source_frameid = std::string(argv[1]);
        std::string target_frameid = std::string(argv[2])+std::to_string(current_pose);;


        // Wait for up to one second for the first transforms to become avaiable.
        echoListener.tf.waitForTransform(source_frameid, target_frameid, ros::Time(), ros::Duration(1.0));

        try
        {
            tf::StampedTransform echo_transform;
            echoListener.tf.lookupTransform(source_frameid, target_frameid, ros::Time(), echo_transform);
            std::cout.precision(precision);
            std::cout.setf(std::ios::fixed,std::ios::floatfield);
            std::cout << "---------------------------------------------------------------------------------- " << std::endl;
            std::cout << " Pose candidate: " << target_frameid << std::endl;

            std::cout << "At time " << echo_transform.stamp_.toSec() << std::endl;
            double yaw, pitch, roll;
            echo_transform.getBasis().getRPY(roll, pitch, yaw);
            tf::Quaternion q = echo_transform.getRotation();
            tf::Vector3 v = echo_transform.getOrigin();
            std::cout << "- Translation: [" << v.getX() << ", " << v.getY() << ", " << v.getZ() << "]" << std::endl;
            std::cout << "- Rotation: in Quaternion [" << q.getX() << ", " << q.getY() << ", "
                      << q.getZ() << ", " << q.getW() << "]" << std::endl
                      << "            in RPY (radian) [" <<  roll << ", " << pitch << ", " << yaw << "]" << std::endl
                      << "            in RPY (degree) [" <<  roll*180.0/M_PI << ", " << pitch*180.0/M_PI << ", " << yaw*180.0/M_PI << "]" << std::endl;


            ss <<"LOCAL CONST robtarget p"<< current_pose <<":=[["<< 1000*v.getX() << "," << 1000*v.getY() << "," << 1000*v.getZ() << "],[" << q.getW() << ","<< q.getX() << "," << q.getY() << "," << q.getZ() << append_str << std::endl;

            ss_aux << "\nRelated with wrist P" << current_pose << std::endl;
            ss_aux << prefix_str << v.getX() << " " << v.getY() << " " << v.getZ() << " " << q.getX() << " " << q.getY() << " " << q.getZ() << " " << q.getW() << append_str_2 << std::endl;
            ss_aux << "------------------------------------------------------------------------------------------------------ " << std::endl;

            ss_aux_raw << "\"" << prefix_str << v.getX() << " " << v.getY() << " " << v.getZ() << " " << q.getX() << " " << q.getY() << " " << q.getZ() << " " << q.getW() << append_str_2 << std::endl;
            ss_aux_raw << "\"" << std::endl;
        }
        catch(tf::TransformException& ex)
        {
            std::cout << "Failure at "<< ros::Time::now() << std::endl;
            std::cout << "Exception thrown:" << ex.what()<< std::endl;
            std::cout << "The current list of frames is:" <<std::endl;
            std::cout << echoListener.tf.allFramesAsString()<<std::endl;

        }
        rate.sleep();
    }

    outfile1 << ss.str();
    outfile1.close();

    outfile2 << ss_aux.str();
    outfile2.close();

    outfile3 << ss_aux_raw.str();
    outfile3.close();

    std::cout << "\nThe output file abb_targets.txt, aux_cmds.txt and aux_cmds_raw were generated in current directory: "<< std::endl;
    system("pwd");
    std::cout << std::endl;

    return 0;
}

