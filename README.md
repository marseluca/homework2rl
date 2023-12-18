<h2>Commands to launch</h2>

<h3>Running Gazebo</h3>
<code>roslaunch iiwa_gazebo iiwa_gazebo_effort.launch</code><br>

<h3>Running the controllers</h3>
<code>rosrun kdl_ros_control kdl_robot_test ./src/iiwa_stack/iiwa_description/urdf/iiwa14.urdf</code>

<h2>KDL robot</h2>
<ul>
  <li><code>kdl_planner.cpp</code>
    <ul>
      <li><code>KDLPlanner::KDLPlanner</code>: constructors</li>
      <li><code>KDLPlanner::trapezoidal_vel</code> and <code>KDLPlanner::cubic_linear</code>: compute the trapezoidal/circular velocity profile via the corresponding equations (<i>Points (1a) and (1b)</i>)</li>
      <li><code>KDLPlanner::path_primitive_linear</code> and <code>KDLPlanner::path_primitive_circular</code>: compute the linear/circular path</li>
      <li><code>KDLPlanner::compute_trapezoidal_linear</code> | <code>compute_trapezoidal_circular</code> | <code>compute_cubic_circular</code> | <code>compute_cubic_linear</code>: combine the <i>profile</i> and the <i>path</i> computation</li>
      <li><code>KDLPlanner::compute_trajectory</code>: computes one of the four above combinations (<i>Points (2) and (3)</i>)</li>
    </ul>
  </li>
  <li><code>kdl_control.cpp</code>
    <ul>
      <li><code>KDLController::idCntr</code>: there are three of them:
        <ul>
          <li>The first one is the implementation of the instructor (joint space inverse dynamics)</li>
          <li>The second one is our implementation (cartesian space inverse dynamics):
            <ol>
              <li>Compute the necessary matrices</li>
              <li>Compute the position, velocity and acceleration error between the current end-effector frame and the desired variables.
                The desired variables <code>_desPos</code>, <code>_desVel</code> and <code>_desAcc</code> are obatined via the computation performed by the <code>KDLPlanner::compute_trajectory</code> function above. NOTE: these represnt the position in the _operational space_</li>
                <li>Compute the algorithm based on the equation in the image below (<i>Point (4)</i>)</li>
            </ol>
          </li>
          <li>The third one is an extra implementation (joint space inverse dynamics - optional)</li>
        </ul>
    </ul>
  </li>
  <li><code>kdl_robot.cpp</code></li>
  <li><code>kdl_robot_test.cpp</code>
    <ul>
      <li><code>KDLPlanner planner;</code>: create a planner object</li>
      <li><code>p = planner.compute_trajectory(0.0,profile,path);</code>: compute the trajectory (cubic/trapezoidal,linear/circular)</li>
      <li><code>des_pose.p = KDL::Vector(p.pos[0],p.pos[1],p.pos[2]);</code>: put the computed trajectory in the <code>des_pose</code> variable (operational space pose)</li>
      <li><code>qd = robot.getInvKin(qd, des_pose);</code>: compute the inverse kinematics to obtain joint space pose from operational space</li>
      <li><code>tau = controller_.idCntr(des_pose, des_cart_vel, des_cart_acc,Kp, Ko, Kdp, 2*sqrt(Ko));</code>: compute the inverse dynamics joint space algorithm</li>
      <li><code>tau = controller_.idCntr(des_pose, des_cart_vel, des_cart_acc,Kp, Kdp);</code>: apply the inverse dynamics operational space algoritm</li>   
    </ul> 
  </li>
</ul>

<h2>Inverse dynamics control (Point 4)</h2>

<h3>Joint space</h3>
Already implemented by the instructor and called via <code>tau = controller_.idCntr(des_pose, des_cart_vel, des_cart_acc,Kp, Kdp);</code>.<br>
Since it's in the joint space, kinematics inversion is needed for obtaining <code>qd</code>, <code>dqd</code> and <code>ddqd</code>, starting from <code>des_pose</code>, <code>des_vel</code> and <code>des_acc</code> respectively.

<h3>Operational space</h3>
Requested in Point 4 and called via <code>tau = controller_.idCntr(des_pose, des_cart_vel, des_cart_acc,Kp, Kdp);</code> through the following equations:
<br><br>

![image](https://github.com/marseluca/homework2rl/assets/33966986/0ea3a39a-3860-47fc-9136-ae5d2b5ef932)

<br>
So the goal is to compute the position and velocity error and put it in the variables <code>x_tilde</code> and <code>dot_x_tilde</code>, altogether with the desired acceleration <code>dot_dot_x_d</code>, starting from <code>des_pose</code>, <code>des_vel</code> and <code>des_acc</code>.
