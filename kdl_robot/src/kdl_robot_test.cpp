#include "kdl_ros_control/kdl_robot.h"
#include "kdl_ros_control/kdl_control.h"
#include "kdl_ros_control/kdl_planner.h"

#include "kdl_parser/kdl_parser.hpp"
#include "urdf/model.h"
#include <std_srvs/Empty.h>
#include <kdl/chainiksolverpos_nr_jl.hpp>
#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/JointState.h"
#include "gazebo_msgs/SetModelConfiguration.h"


// Global variables
std::vector<double> jnt_pos(7,0.0), jnt_vel(7,0.0), obj_pos(6,0.0),  obj_vel(6,0.0);
bool robot_state_available = false;

// Functions
KDLRobot createRobot(std::string robot_string)
{
    KDL::Tree robot_tree;
    urdf::Model my_model;
    if (!my_model.initFile(robot_string))
    {
        printf("Failed to parse urdf robot model \n");
    }
    if (!kdl_parser::treeFromUrdfModel(my_model, robot_tree))
    {
        printf("Failed to construct kdl tree \n");
    }
    
    KDLRobot robot(robot_tree);
    return robot;
}

void jointStateCallback(const sensor_msgs::JointState & msg)
{
    robot_state_available = true;
    jnt_pos.clear();
    jnt_vel.clear();
    for (int i = 0; i < msg.position.size(); i++)
    {
        jnt_pos.push_back(msg.position[i]);
        jnt_vel.push_back(msg.velocity[i]);
    }
}

// Main
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Please, provide a path to a URDF file...\n");
        return 0;
    }

    // Init node
    ros::init(argc, argv, "kdl_ros_control_node");
    ros::NodeHandle n;

    // Rate
    ros::Rate loop_rate(500);

    // Subscribers
    ros::Subscriber joint_state_sub = n.subscribe("/iiwa/joint_states", 1, jointStateCallback);

    // Publishers
    
    // Joints torques
    ros::Publisher joint1_effort_pub = n.advertise<std_msgs::Float64>("/iiwa/iiwa_joint_1_effort_controller/command", 1);
    ros::Publisher joint2_effort_pub = n.advertise<std_msgs::Float64>("/iiwa/iiwa_joint_2_effort_controller/command", 1);
    ros::Publisher joint3_effort_pub = n.advertise<std_msgs::Float64>("/iiwa/iiwa_joint_3_effort_controller/command", 1);
    ros::Publisher joint4_effort_pub = n.advertise<std_msgs::Float64>("/iiwa/iiwa_joint_4_effort_controller/command", 1);
    ros::Publisher joint5_effort_pub = n.advertise<std_msgs::Float64>("/iiwa/iiwa_joint_5_effort_controller/command", 1);
    ros::Publisher joint6_effort_pub = n.advertise<std_msgs::Float64>("/iiwa/iiwa_joint_6_effort_controller/command", 1);
    ros::Publisher joint7_effort_pub = n.advertise<std_msgs::Float64>("/iiwa/iiwa_joint_7_effort_controller/command", 1);    
    
    // Joints desired positions
    ros::Publisher joint1_qd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint1_desired_position", 1);
    ros::Publisher joint2_qd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint2_desired_position", 1);
    ros::Publisher joint3_qd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint3_desired_position", 1);
    ros::Publisher joint4_qd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint4_desired_position", 1);
    ros::Publisher joint5_qd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint5_desired_position", 1);
    ros::Publisher joint6_qd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint6_desired_position", 1);    
    ros::Publisher joint7_qd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint7_desired_position", 1);
    
    // Joints desired velocities
    ros::Publisher joint1_dqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint1_desired_velocity", 1);
    ros::Publisher joint2_dqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint2_desired_velocity", 1);
    ros::Publisher joint3_dqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint3_desired_velocity", 1);
    ros::Publisher joint4_dqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint4_desired_velocity", 1);
    ros::Publisher joint5_dqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint5_desired_velocity", 1);
    ros::Publisher joint6_dqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint6_desired_velocity", 1);    
    ros::Publisher joint7_dqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint7_desired_velocity", 1);
    
    // Joints desired accelerations
    ros::Publisher joint1_ddqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint1_desired_acceleration", 1);
    ros::Publisher joint2_ddqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint2_desired_acceleration", 1);
    ros::Publisher joint3_ddqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint3_desired_acceleration", 1);
    ros::Publisher joint4_ddqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint4_desired_acceleration", 1);
    ros::Publisher joint5_ddqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint5_desired_acceleration", 1);
    ros::Publisher joint6_ddqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint6_desired_acceleration", 1);    
    ros::Publisher joint7_ddqd_pub = n.advertise<std_msgs::Float64>("/iiwa/joint7_desired_acceleration", 1);

    // errors
    ros::Publisher joint1_err = n.advertise<std_msgs::Float64>("/iiwa/joint1_err", 1);
    ros::Publisher joint2_err = n.advertise<std_msgs::Float64>("/iiwa/joint2_err", 1);
    ros::Publisher joint3_err = n.advertise<std_msgs::Float64>("/iiwa/joint3_err", 1);
    ros::Publisher joint4_err = n.advertise<std_msgs::Float64>("/iiwa/joint4_err", 1);
    ros::Publisher joint5_err = n.advertise<std_msgs::Float64>("/iiwa/joint5_err", 1);
    ros::Publisher joint6_err = n.advertise<std_msgs::Float64>("/iiwa/joint6_err", 1);    
    ros::Publisher joint7_err = n.advertise<std_msgs::Float64>("/iiwa/joint7_err", 1);
     //norm error
    ros::Publisher norm_err = n.advertise<std_msgs::Float64>("/iiwa/norm_error", 1);
    // Services
    ros::ServiceClient robot_set_state_srv = n.serviceClient<gazebo_msgs::SetModelConfiguration>("/gazebo/set_model_configuration");
    ros::ServiceClient pauseGazebo = n.serviceClient<std_srvs::Empty>("/gazebo/pause_physics");

    // Set robot state
    gazebo_msgs::SetModelConfiguration robot_init_config;
    robot_init_config.request.model_name = "iiwa";
    robot_init_config.request.urdf_param_name = "robot_description";
    robot_init_config.request.joint_names.push_back("iiwa_joint_1");
    robot_init_config.request.joint_names.push_back("iiwa_joint_2");
    robot_init_config.request.joint_names.push_back("iiwa_joint_3");
    robot_init_config.request.joint_names.push_back("iiwa_joint_4");
    robot_init_config.request.joint_names.push_back("iiwa_joint_5");
    robot_init_config.request.joint_names.push_back("iiwa_joint_6");
    robot_init_config.request.joint_names.push_back("iiwa_joint_7");
    robot_init_config.request.joint_positions.push_back(0.0);
    robot_init_config.request.joint_positions.push_back(1.57);
    robot_init_config.request.joint_positions.push_back(-1.57);
    robot_init_config.request.joint_positions.push_back(-1.2);
    robot_init_config.request.joint_positions.push_back(1.57);
    robot_init_config.request.joint_positions.push_back(-1.57);
    robot_init_config.request.joint_positions.push_back(-0.37);
    if(robot_set_state_srv.call(robot_init_config))
        ROS_INFO("Robot state set.");
    else
        ROS_INFO("Failed to set robot state.");

    // Messages
    std_msgs::Float64 tau1_msg, tau2_msg, tau3_msg, tau4_msg, tau5_msg, tau6_msg, tau7_msg;
    std_msgs::Float64 qd1_msg, qd2_msg, qd3_msg, qd4_msg, qd5_msg, qd6_msg, qd7_msg;
    std_msgs::Float64 dqd1_msg, dqd2_msg, dqd3_msg, dqd4_msg, dqd5_msg, dqd6_msg, dqd7_msg;
    std_msgs::Float64 ddqd1_msg, ddqd2_msg, ddqd3_msg, ddqd4_msg, ddqd5_msg, ddqd6_msg, ddqd7_msg;
    std_msgs::Float64 err1_msg, err2_msg, err3_msg, err4_msg, err5_msg, err6_msg, err7_msg;
    std_msgs::Float64 norm_msg;
    
    std_srvs::Empty pauseSrv;

    // Wait for robot and object state
    while (!(robot_state_available))
    {
        ROS_INFO_STREAM_ONCE("Robot/object state not available yet.");
        ROS_INFO_STREAM_ONCE("Please start gazebo simulation.");
        if (!(robot_set_state_srv.call(robot_init_config)))
            ROS_INFO("Failed to set robot state.");            
        
        ros::spinOnce();
    }

    // Create robot
    KDLRobot robot = createRobot(argv[1]);
    robot.update(jnt_pos, jnt_vel);
    int nrJnts = robot.getNrJnts();

    // Specify an end-effector 
    robot.addEE(KDL::Frame::Identity());

    // Joints
    KDL::JntArray qd(robot.getNrJnts()),dqd(robot.getNrJnts()),ddqd(robot.getNrJnts());
    dqd.data.setZero();
    ddqd.data.setZero();

    // Torques
    Eigen::VectorXd tau;
    tau.resize(robot.getNrJnts());

    // Update robot
    robot.update(jnt_pos, jnt_vel);

    // Init controller
    KDLController controller_(robot);

    // Object's trajectory initial position
    KDL::Frame init_cart_pose = robot.getEEFrame();
    Eigen::Vector3d init_position(init_cart_pose.p.data);

    // Object trajectory end position
    Eigen::Vector3d end_position;
    end_position << init_cart_pose.p.x(), -init_cart_pose.p.y(), init_cart_pose.p.z();

    // Plan trajectory
    double traj_duration = 5, acc_duration = 0.7, t = 0.0, init_time_slot = 1.0, radius=0.08;

    // LINEAR TRAJECTORY CONSTRUCTOR
     //KDLPlanner planner(traj_duration, acc_duration, init_position, end_position); 

    // CIRCULAR TRAJECTORY CONSTRUCTOR
    //KDLPlanner planner(traj_duration, init_position, radius);

    //GENERAL CONSTRUCTOR
    KDLPlanner planner(traj_duration, acc_duration, init_position, end_position,radius);
    // Retrieve the first trajectory point
    std::string profile="cubic";
    std::string path="linear";
    trajectory_point p = planner.compute_trajectory(t,profile,path);

    // Gains
    double Kp = 150, Kd = 72;

    // Retrieve initial simulation time
    ros::Time begin = ros::Time::now();
    ROS_INFO_STREAM_ONCE("Starting control loop ...");

    // Init trajectory
    KDL::Frame des_pose = KDL::Frame::Identity(); 
    KDL::Twist des_cart_vel = KDL::Twist::Zero(), des_cart_acc = KDL::Twist::Zero();
    des_pose.M = robot.getEEFrame().M;

    while ((ros::Time::now()-begin).toSec() < 2*traj_duration + init_time_slot)
    {
        if (robot_state_available)
        {
            // Update robot
            robot.update(jnt_pos, jnt_vel);

            // Update time
            t = (ros::Time::now()-begin).toSec();
            std::cout << "time: " << t << std::endl;

            // Extract desired pose
            des_cart_vel = KDL::Twist::Zero();
            des_cart_acc = KDL::Twist::Zero();
            if (t <= init_time_slot) // wait a second
            {
                p = planner.compute_trajectory(0.0,profile,path);
            }
            else if(t > init_time_slot && t <= traj_duration + init_time_slot)
            {
                p = planner.compute_trajectory(t-init_time_slot,profile,path);
                des_cart_vel = KDL::Twist(KDL::Vector(p.vel[0], p.vel[1], p.vel[2]),KDL::Vector::Zero());
                des_cart_acc = KDL::Twist(KDL::Vector(p.acc[0], p.acc[1], p.acc[2]),KDL::Vector::Zero());
            }
            else
            {
                ROS_INFO_STREAM_ONCE("trajectory terminated");
                break;
            }

            des_pose.p = KDL::Vector(p.pos[0],p.pos[1],p.pos[2]);
            
            
            // std::cout << "jacobian: " << std::endl << robot.getEEJacobian().data << std::endl;
            // std::cout << "jsim: " << std::endl << robot.getJsim() << std::endl;
            // std::cout << "c: " << std::endl << robot.getCoriolis().transpose() << std::endl;
            // std::cout << "g: " << std::endl << robot.getGravity().transpose() << std::endl;
            // std::cout << "qd: " << std::endl << qd.data.transpose() << std::endl;
            // std::cout << "q: " << std::endl << robot.getJntValues().transpose() << std::endl;
            // std::cout << "tau: " << std::endl << tau.transpose() << std::endl;
            // std::cout << "desired_pose: " << std::endl << des_pose << std::endl;
            // std::cout << "current_pose: " << std::endl << robot.getEEFrame() << std::endl;

            // inverse kinematics
            qd.data << jnt_pos[0], jnt_pos[1], jnt_pos[2], jnt_pos[3], jnt_pos[4], jnt_pos[5], jnt_pos[6];
            qd = robot.getInvKin(qd, des_pose);
            dqd=robot.getInvKinVel(qd,des_cart_vel);
            // joint space inverse dynamics control
          // tau = controller_.idCntr(qd, dqd, ddqd, Kp, Kd);
            
            double Kp = 80;
            double Ko = 50;
            double Kdp = 40;

            // Cartesian space inverse dynamics control
            tau = controller_.idCntr(des_pose, des_cart_vel, des_cart_acc,
                                     Kp, Ko, Kdp, 2*sqrt(Ko));
            //CArtesian space inverse dynamics controll exploiting redundancy, we do not assign the orientation
           //  tau = controller_.idCntr(des_pose, des_cart_vel, des_cart_acc,
            //                          Kp, Kdp);                          
            Eigen::VectorXd errors =qd.data-robot.getJntValues();
            // Set torques
            tau1_msg.data = tau[0];
            tau2_msg.data = tau[1];
            tau3_msg.data = tau[2];
            tau4_msg.data = tau[3];
            tau5_msg.data = tau[4];
            tau6_msg.data = tau[5];
            tau7_msg.data = tau[6];
            
            //creating message desired joints positions
            qd1_msg.data=qd.data[0];
            qd2_msg.data=qd.data[1];
            qd3_msg.data=qd.data[2];
            qd4_msg.data=qd.data[3];
            qd5_msg.data=qd.data[4];
            qd6_msg.data=qd.data[5];
            qd7_msg.data=qd.data[6];
            
            //creating message desired joints velocities
            dqd1_msg.data=dqd.data[0];
            dqd2_msg.data=dqd.data[1];
            dqd3_msg.data=dqd.data[2];
            dqd4_msg.data=dqd.data[3];
            dqd5_msg.data=dqd.data[4];
            dqd6_msg.data=dqd.data[5];
            dqd7_msg.data=dqd.data[6];
            
            //creating message desired joints accelerations
            ddqd1_msg.data=ddqd.data[0];
            ddqd2_msg.data=ddqd.data[1];
            ddqd3_msg.data=ddqd.data[2];
            ddqd4_msg.data=ddqd.data[3];
            ddqd5_msg.data=ddqd.data[4];
            ddqd6_msg.data=ddqd.data[5];
            ddqd7_msg.data=ddqd.data[6];
            //creating message errors
            err1_msg.data=errors[0];
            err2_msg.data=errors[1];;
            err3_msg.data=errors[2];;
            err3_msg.data=errors[3];;
            err5_msg.data=errors[4];;
            err6_msg.data=errors[5];;
            err7_msg.data=errors[6];;
            //creating error norm msg
            norm_msg.data=errors.norm();
            // Publish
            joint1_effort_pub.publish(tau1_msg);
            joint2_effort_pub.publish(tau2_msg);
            joint3_effort_pub.publish(tau3_msg);
            joint4_effort_pub.publish(tau4_msg);
            joint5_effort_pub.publish(tau5_msg);
            joint6_effort_pub.publish(tau6_msg);
            joint7_effort_pub.publish(tau7_msg);
            
            //publishing desired joint positions
            joint1_qd_pub.publish(qd1_msg);
            joint2_qd_pub.publish(qd2_msg);
            joint3_qd_pub.publish(qd3_msg);
            joint4_qd_pub.publish(qd4_msg);
            joint5_qd_pub.publish(qd5_msg);
            joint6_qd_pub.publish(qd6_msg);
            joint7_qd_pub.publish(qd7_msg);
            
            // publishing desired joint velocities
            joint1_dqd_pub.publish(dqd1_msg);
            joint2_dqd_pub.publish(dqd2_msg);
            joint3_dqd_pub.publish(dqd3_msg);
            joint4_dqd_pub.publish(dqd4_msg);
            joint5_dqd_pub.publish(dqd5_msg);
            joint6_dqd_pub.publish(dqd6_msg);
            joint7_dqd_pub.publish(dqd7_msg);

             // publishing desired joint acceleration
            joint1_ddqd_pub.publish(ddqd1_msg);
            joint2_ddqd_pub.publish(ddqd2_msg);
            joint3_ddqd_pub.publish(ddqd3_msg);
            joint4_ddqd_pub.publish(ddqd4_msg);
            joint5_ddqd_pub.publish(ddqd5_msg);
            joint6_ddqd_pub.publish(ddqd6_msg);
            joint7_ddqd_pub.publish(ddqd7_msg);
             // publishing joint error
            joint1_err.publish(err1_msg);
            joint2_err.publish(err2_msg);
            joint3_err.publish(err3_msg);
            joint4_err.publish(err4_msg);
            joint5_err.publish(err5_msg);
            joint6_err.publish(err6_msg);
            joint7_err.publish(err7_msg);
            //norm error
            norm_err.publish(norm_msg);
                        
            ros::spinOnce();
            loop_rate.sleep();
        }
    }
    if(pauseGazebo.call(pauseSrv))
        ROS_INFO("Simulation paused.");
    else
        ROS_INFO("Failed to pause simulation.");

    return 0;
}