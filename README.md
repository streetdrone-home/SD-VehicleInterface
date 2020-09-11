
## The StreetDrone Vehicle Interface
This package is responsible for the communication between the [StreetDrone](https://streetdrone.com/) vehicles and ROS based self-driving software stacks.

The interface bridges the gap between ROS kinetic and the OpenCAN vehicle interface of the StreetDrone Xenos Control Unit (XCU) integrated into the SD Twizy R&D and SD ENV200 vehicles.

### Disclaimer:
We strongly suggest that you adhere to the following guideline in conjuction with the documentation you received alongside your vehicle
* A trained safety driver must always be present in the vehicle, monitoring the behavior of the vehicle and ready to take control at any time.
* This software is for research purposes only. Access to and use of this software is at your own risk and responsibility. No warranty expressed or implied.
* You are responsible for complying with the local laws and regulations.

#### In this release:
* A tunable PID and FeedForward Linear Velocity Control loop has been implemened with a mature calibration at speeds of up to 20mph. 
* Support has been extended to the ENV200, the latest vehicle in the StreetDrone fleet
* An intuitive yaw to steering map has been included
* Support for Localization, CAN and IMU speed source selection
* Support for OXTS and PEAK GPS-IMU
* Simulation mode for [SD-TwizyModel](https://github.com/streetdrone-home/SD-TwizyModel) in Gazebo
* Minor bug fixes

#### Architecture
*sd_vehicle_interface:* This node is responsible for the main functionality of the package. Here the autonomous to manual mode handshake verification with the vehicle is implemented. 
	The output of the `/sd_diagnostics` topic is used to implement a fault response if an issue is detected in the software.

*sd_control:* A tunable PID and feedforward control function for the StreetDrone Twizy and ENV200. A function for mapping angular velocity request from the software to the vehicle's steering angle is also implemented. 

*sd_gps_imu:* The CAN parsing functions for an OXTS GPS and PEAK CAN GPS-IMU. 

*sd_lib:* The implementation of the StreetDrone CAN protocol.

*sd_typdefs:* Variables and constants definitions.

#### Node Architecture
socketcan_bridge: This node comes from the default ROS package [socketcan_bridge](http://wiki.ros.org/socketcan_bridge). The package provides functionality to expose CAN frames from SocketCAN to a ROS Topic. Internally it uses the socketcan_interface from the ros_canopen package, as such, it is capable of dealing with both normal and extended CAN frames. 

sd_vehicle_interface: The vehicle interface node for the StreetDrone (SD) Twizy and ENV200. This node translates the output messages from a ROS based software stack (geometry_msgs::TwistStamped) to SocketCAN messages (can_msgs::Frame) and vice versa. The node integrates a twist to ackermann function for controlling the steering of the vehicle and a PID controller with vehicle speed feedback corresponding to the output twist message.  

#### Limitations:
* limited to 20 Mph

## Requirements

##### - Ubuntu 16.04 LTS
##### - ROS Kinetic [ros-kinetic-desktop-full](http://wiki.ros.org/kinetic/Installation/Ubuntu)
##### - Catkin Command Line Tools [catkin_tools](https://catkin-tools.readthedocs.io/en/latest/installing.html)

## Building

1. Go to the `/src` directory of your catkin workspace and clone this repository or copy its contents.   
If you haven't previously created a catkin workspace, please visit [wiki.ros.org/Tutorials/create_a_workspace](http://wiki.ros.org/catkin/Tutorials/create_a_workspace) 

2. Install the ROS packages: [socketcan_interface](http://wiki.ros.org/socketcan_interface) and [can_msgs](http://wiki.ros.org/can_msgs)
```
sudo apt-get install ros-kinetic-socketcan-interface ros-kinetic-can-msgs
```

3. Build the package
```
# from the root of your workspace build only this package
catkin build sd_vehicle_interface
```

If you previously built your workspace with `catkin_make`, do `catkin_make --only-pkg-with-deps sd_vehicle_interface`.    

Launch
------
Before launching, ensure that the CAN interface has been initialised.  
For PEAK CAN USB interfaces, the steps to initialise CAN as can0, are:
```
sudo modprobe peak_usb
sudo ip link set can0 up type can bitrate 500000
```
If not connected to the car, virtual CAN Bus can be used. The steps to initialise virtual CAN as vcan, are:
```
sudo modprobe vcan
sudo ip link add dev can0 type vcan
```
After CAN is initialised, go to the root of your workspace and launch the vehicle interface using the following command:
```
source devel/setup.bash
roslaunch sd_vehicle_interface sd_vehicle_interface.launch sd_vehicle:=env200 sd_gps_imu:=oxts
# adjust the launch parameters to your vehicle setup, as described below
```

Parameters
------

| arg                | values                                    | default           | description                                |
|--------------------|-------------------------------------------|-------------------|--------------------------------------------|
| sd_vehicle         | {env200,twizy}                            | env200            | The vehicle under control                  |
| sd_gps_imu         | {oxts, peak, none}                        | oxts              | The GPS/IMU used                           |
| sd_speed_source    | {vehicle_can_speed, imu_speed, ndt_speed} | vehicle_can_speed | Input vehicle speed                        |
| sd_simulation_mode | {true, false}                             | false             | Use on the car or on the Gazebo simulation |
