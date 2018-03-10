these files is the packages used to communicate with RoboMaster Robot.

To add the packages to your ROS project, you should copy these five documents to
%Path To Your Workspace%/src  path, and type "catkin_make" command.

To run the demo, You can type "roslaunch rm_control_demo demo.launch" and then,
the Robot's states like Gimbal Angles, IMU Angles, Chassis Motor Speed, Module Error Info
, Remote Controller Info and Odometer Info.
will be pulished.You can read them at command window, in addition, for example,  you can type command " rostopic echo  /RobotIMU" to get the IMU Info.

Send messages to "/RobotControl" topic to control the Gimbal and the Chassis of the Robot,what you can control are
1.Chassis_x_speed(mm/s)
2.Chassis_y_speed(mm/s)
3.Chassis_rotate_speed(rad/s)
4.rads that Gimbal pitch angle should be adjusted (rads)
5.rads that Gimbal yaw angle should be adjusted (rads)

compile robot_protocol_msgs first, then compile rest

Note: Please adjust the current time of mainfold before compiling.


