

## Calibration performed in this directory
The setup used is presented bellow


###### 1. Calibrate the robot probe tool

###### 2. ~~ Run the CharucoDetector and verify the measurement (position) of Charuco with the calibrated probe tool(PB). Try to adjust the Charuco parameters (into the launch file) to minimize the error relationship. ~~

###### 3. With the robot probe tool, create a external working frame guided by the Charuco Frame and its plane orientation. Now you have a suppose Charuco pose in space (lest call it simulated charuco). This is just a guide to create a understandable calibration points. Save this external axis on ```generate_charuco_calib_points.launch``` such as:

```
<node pkg="tf" type="static_transform_publisher" name="static_transform_publisher_ch" args=" .170572 -.888895 0.0427575 -0.00517996 -0.0052119 0.710179 0.703983 robot_base charuco 100" />
```

###### 4. Put the robot in a pose that (lets call this as wrist_0):
- the TCP0/Tool0/Wrist has the same planar face with the the simulated charuco plane;
- the high is enough to get a good image of the charuco, typically centralized over the charuco board;

###### 5. In the robot, get the pose of TCP0/Tool0/Wrist pose w.r.t robot_base. Save this pose on ```generate_charuco_calib_points.launch``` such as:

```
<node pkg="tf" type="static_transform_publisher" name="static_transform_publisher_0"  args=" 0.199821429 0.27975 0.5366 -0.99974 -0.00204 0.01545 0.01682 robot_base wrist_0 100" />
```

###### 6. Now, create manually frames (lets call it "wrists_i-th") related to wrist_0 (or any other frame that facilitates the designing), such as:

```
	<node pkg="tf" type="static_transform_publisher" name="static_transform_publisher_3"  args=" 0 0 0 0 0 1 0  wrist_0 wrist_3 100" />
	<node pkg="tf" type="static_transform_publisher" name="static_transform_publisher_4"  args="0.14025 0 0 0 -0.0610485 0 0.9981348 wrist_0 wrist_4 100" /> 	<!-- -7 ry -->
```

- Normally set around 7deg of transversal orientations and -90,0,90,180 of plane orientations related to simulated charuco pose. It is useful to get one per vertex and size of charuco.

###### 7. Run ```generate_charuco_calib_points.launch``` and verify in RViz the created frames.

###### 8. With the ```rosrun tf tf_echo robot_base wrist_x``` get all wrists poses w.r.t robot pose. Create a robot program with these point and load the program into robot. This will be latter used by the easy_handeye_algorithm.

This procedure can be performed automatically running the ros node ```generate_abb_code```. Such as:
```
rosrun calib_camera_external generate_abb_code robot_base wrist_ 20
```

- For this, the second parameter should be the base name of the wrists frames and the third the total number of frames, i.e. all frames should have a base_name + #index where, index = 0 to total number of frames.

- This node will generate a abb_code.txt, such as ```docs\abb_targets```, with all wrists pose w.r.t robot base in RAPID data structure and a aux.txt,such as,```docs\aux_file_abb``` with the rosrun command to be used by the next step. Be aware that all writs are named as tool0 in file. This will be latter used by the easy_handeye_algorithm.

###### 9. Run the abb_calib.launch.

###### 10. Put the robot in ith-pose and run the correspondent line code of the aux.file and capture by clicking in rqt/rviz guide. Repeat this procedure untill the end.
- This procedure can be done by running the ```update_tf.sh script``` with the updated poses
- This procedure is manually performed since the abb-1600 is not directly connected to the easy_handeye.

###### 11. Get the calibrated pose of the camera w.r.t TCP0/Tool0/Wrist by clicking in calculate.

###### 12. Put this calibrated pose, the wrist_0 pose and the calibrated tool probe pose in ```verify_easyhandeye_result.launch``` (see below) and verify the result:
```
<!--position P0 in TP (pose of TCP0 during the CharucoPoseEstimation). The robot must be in the position P0 to corrected measurements-->
<node pkg="tf" type="static_transform_publisher" name="static_transform_publisher_p0"  args="-.118774 -.691591 .575185 0.702517  0.711436  -0.017622 0.004321 robot_base tool0 100" />

<!--by calibrating the robot effective tool-->
<node pkg="tf" type="static_transform_publisher" name="static_transform_publisher_ponta_de_prova"  args="-.00526911 0.296272 0.305406 -0.00668129 0.00893221 0.708459 0.705664  tool0 ponta_prova 100" />

<!-- camera_link pose to tool0. Estimated by easyhandeye -->
<node pkg="tf" type="static_transform_publisher" name="static_transform_publisher_camera_d435"  args=" -0.016651746849288683 0.23713801801978257 0.1589293391764847 -0.0053121823096013866 -0.7018028633858873 -0.01548127957138288 0.7121831587768082 tool0 camera_link 100" />

```
###### 13. Put this calibrated camera pose back to robot controller.

## NOTE:
RAPID ABB uses ZYX and quaternion format q1:(w) q2:(x) q3:(y) q4:(z)
