#!/bin/bash

command_arr=(
"rosrun tf static_transform_publisher -0.2512 -0.72377 0.58373 0.715272 0.698776 0.00625841 0.00762991 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.2512 -0.72377 0.58373 0.0116644 0.999883 -0.000969797 0.00982053 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.2512 -0.72377 0.58373 0.999883 -0.0116644 0.00982053 0.000969797 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.2512 -0.72377 0.58373 -0.698776 0.715272 -0.00762991 0.00625841 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.246535 -0.524003 0.583388 0.69252 0.672991 -0.179081 0.188227 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.255865 -0.923537 0.584072 0.68928 0.676941 0.191171 -0.173487 robot_base tool0 100
"
"rosrun tf static_transform_publisher 0.0284199 -0.73029 0.58923 0.692875 0.676586 -0.174811 -0.177756 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.53082 -0.71725 0.57823 0.688925 0.673346 0.186902 0.192496 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.526155 -0.517483 0.577888 0.620206 0.696409 0.00120407 0.36105 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.535485 -0.917017 0.578571 0.710695 0.604396 0.359862 0.0108236 robot_base tool0 100
"
"rosrun tf static_transform_publisher 0.023755 -0.930057 0.589572 0.620892 0.703353 0.00945207 -0.345974 robot_base tool0 100
"
"rosrun tf static_transform_publisher 0.0330848 -0.530523 0.588889 0.717639 0.60371 -0.347162 0.00257557 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.246535 -0.524003 0.583388 0.965562 -0.0138087 0.00646692 0.259726 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.255865 -0.923537 0.584072 0.966064 -0.00872524 0.0125049 -0.257852 robot_base tool0 100
"
"rosrun tf static_transform_publisher 0.0284199 -0.73029 0.58923 0.968355 -0.011518 -0.249303 -0.00208223 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.53082 -0.71725 0.57823 0.963271 -0.011016 0.268275 0.00395573 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.526155 -0.517483 0.577888 0.930988 0.0538837 0.256152 0.25445 robot_base tool0 100
"
"rosrun tf static_transform_publisher -0.535485 -0.917017 0.578571 0.92991 -0.0751649 0.262115 -0.246808 robot_base tool0 100
"
"rosrun tf static_transform_publisher 0.023755 -0.930057 0.589572 0.936383 0.0583091 -0.237957 -0.251324 robot_base tool0 100
"
"rosrun tf static_transform_publisher 0.0330848 -0.530523 0.588889 0.934335 -0.0805601 -0.243659 0.247302 robot_base tool0 100
"
)

declare -i COUNTER=-1
declare -i MAX=${#command_arr[@]}

name_str="temp_static_pub_node"
name_cmd=" __name:="$name_str"_"

echo "Press any key to continue"
while [ true ] ; do
	read -t 3 -n 1
	if [ $? = 0 ] ; then
		let COUNTER++
		printf "Setting TF command to #%d\n" $COUNTER
		if [ $COUNTER -eq $MAX ]; then
			echo "Finished"
			rosnode kill $(rosnode list | grep $name_str)
			exit

		else
			printf "Publishing TF command #%d\n" $COUNTER

			if [ $COUNTER -eq 0 ]; then
				eval ${command_arr[$COUNTER]}$name_cmd$COUNTER& tf_pub_pid=$!
			else
				rosnode kill $(rosnode list | grep $name_str)&&  eval ${command_arr[$COUNTER]}$name_cmd$COUNTER& tf_pub_pid=$!
			fi
		fi
	fi
done

# rosnode kill $(rosnode list | grep static*)
# kill tf_pub_pid does not work.. PID is not correct with ros nodes...
