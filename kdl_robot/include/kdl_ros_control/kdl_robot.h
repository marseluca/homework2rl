#ifndef KDLROBOT
#define KDLROBOT

#include <kdl/chain.hpp>
#include <kdl/chaindynparam.hpp>
#include <kdl/chainiksolverpos_nr_jl.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/chainjnttojacsolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainfksolvervel_recursive.hpp>
#include <kdl/chainjnttojacdotsolver.hpp>
#include <kdl/chainidsolver_recursive_newton_euler.hpp>
#include <kdl/chainiksolvervel_wdls.hpp>
#include <kdl/tree.hpp>
#include <kdl/frames.hpp>
#include <kdl/framevel.hpp>
#include <kdl/frames_io.hpp>

#include "utils.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

class KDLRobot
{

public:
    double epsilon_t;
    // robot
    KDLRobot();
    KDLRobot(KDL::Tree &robot_tree);
    void update(std::vector<double> _jnt_values,std::vector<double> _jnt_vel);
    unsigned int getNrJnts();
    unsigned int getNrSgmts();
    void addEE(const KDL::Frame &_f_tip);

    // joints
    Eigen::MatrixXd getJntLimits();
    Eigen::MatrixXd getJsim();
    Eigen::MatrixXd getCoriolisMatrix();
    Eigen::VectorXd getCoriolis();
    Eigen::VectorXd getGravity();
    Eigen::VectorXd getJntValues();
    Eigen::VectorXd getJntVelocities();
    KDL::ChainIdSolver_RNE* idSolver_;
    Eigen::VectorXd getID(const KDL::JntArray &q,
                          const KDL::JntArray &q_dot,
                          const KDL::JntArray &q_dotdot,
                          const KDL::Wrenches &f_ext);

    KDL::ChainIkSolverPos_NR_JL* ikSol_;
    KDL::JntArray getInvKin(const KDL::JntArray &q,
                            const KDL::Frame &eeFrame);
    KDL::JntArray getInvKinVel(const KDL::JntArray &qd,
                        const KDL::Twist &eeFrameVel);
    Eigen::Matrix<double,7,1> getInvKinAcc(const KDL::Twist &eeFrameAcc,const KDL::JntArray &dqd,
                                Eigen::Matrix<double,6,7> J,Eigen::Matrix<double,6,7> Jdot);
    // end-effector
    KDL::Frame getEEFrame();
    KDL::Frame getFlangeEE();
    KDL::Twist getEEVelocity();
    KDL::Twist getEEBodyVelocity();
    KDL::Jacobian getEEJacobian();
    KDL::Jacobian getEEBodyJacobian();
    Eigen::VectorXd getEEJacDotqDot();
    Eigen::VectorXd getEEJacDotqDot_red();


    void getInverseKinematics(KDL::Frame &f,
                              KDL::Twist &twist,
                              KDL::Twist &acc,
                              KDL::JntArray &q,
                              KDL::JntArray &dq,
                              KDL::JntArray &ddq);

private:

    // chain
    unsigned int n_;
    void createChain(KDL::Tree &robot_tree);
    KDL::Chain chain_;
    KDL::ChainDynParam* dynParam_;
    KDL::ChainJntToJacSolver* jacSol_;
    KDL::ChainFkSolverPos_recursive* fkSol_;
    KDL::ChainFkSolverVel_recursive* fkVelSol_;
    KDL::ChainJntToJacDotSolver* jntJacDotSol_;
    // KDL::ChainIkSolverPos_NR_JL* ikSol_;
    KDL::ChainIkSolverVel_wdls* ikVelSol_;

    // joints
    void updateJnts(std::vector<double> _jnt_values, std::vector<double> _jnt_vel);
    KDL::JntSpaceInertiaMatrix jsim_;
    KDL::JntArray jntArray_;
    KDL::JntArray jntVel_;
    KDL::JntArray coriol_;
    KDL::JntArray grav_;
    KDL::JntArray q_min_;
    KDL::JntArray q_max_;

    // end-effector
    KDL::Frame f_F_ee_;             // end-effector frame in flange frame
    KDL::Frame s_F_ee_;             // end-effector frame in spatial frame
    KDL::Twist s_V_ee_;             // end-effector twist in spatial frame
    KDL::Jacobian s_J_ee_;          // end-effector Jacobian in spatial frame
    KDL::Jacobian b_J_ee_;          // end-effector Jacobian in body frame
    KDL::Jacobian s_J_dot_ee_;      // end-effector Jacobian dot in spatial frame
    KDL::Jacobian b_J_dot_ee_;      // end-effector Jacobian dot in body frame
    KDL::Twist s_J_dot_q_dot_ee_;   // end-effector Jdot*qdot in spatial frame

};

#endif
