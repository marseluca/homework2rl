close,clear
clc

bag=rosbag('my_rosbag.bag');

bSel_1=select(bag,'Topic', '/iiwa/iiwa_joint_1_effort_controller/command');
bSel_2=select(bag,'Topic', '/iiwa/iiwa_joint_2_effort_controller/command');
bSel_3=select(bag,'Topic', '/iiwa/iiwa_joint_3_effort_controller/command');
bSel_4=select(bag,'Topic', '/iiwa/iiwa_joint_4_effort_controller/command');
bSel_5=select(bag,'Topic', '/iiwa/iiwa_joint_5_effort_controller/command');
bSel_6=select(bag,'Topic', '/iiwa/iiwa_joint_6_effort_controller/command');
bSel_7=select(bag,'Topic', '/iiwa/iiwa_joint_7_effort_controller/command');

EJ1=timeseries(bSel_1);
EJ2=timeseries(bSel_2);
EJ3=timeseries(bSel_3);
EJ4=timeseries(bSel_4);
EJ5=timeseries(bSel_5);
EJ6=timeseries(bSel_6);
EJ7=timeseries(bSel_7);

figure
plot(EJ1)
hold on
plot(EJ2)
hold on
plot(EJ3)
hold on
plot(EJ4)
hold on
plot(EJ5)
hold on
plot(EJ6)
hold on
plot(EJ7)
hold on


