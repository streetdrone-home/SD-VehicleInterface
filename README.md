## The StreetDrone Vehicle Interface
This package is responsible for the communication between the [StreetDrone](https://streetdrone.com/) Twizy vehicle and [Autoware](https://github.com/autowarefoundation/autoware). The sd package needs to be built from inside Autoware thus the latest version of Autoware needs to be cloned in your local machine. For more details please see below.

The interface is wrapped in a shared library `.so` binary format and intends to bridge the gap between Autoware and the OpenCAN vehicle interface of the StreetDrone Xenos Control Unit (XCU) integrated into the SD Twizy R&D vehicle.

### Disclaimer:
We strongly suggest that you adhere to the following guidelines in conjuction with the documentation you received alongside your SD Twizy
* A trained safety driver must always be present in the vehicle in order to provide critical redundancy
* The current release of the vehicle interface is **not** suitable for use on public roads. It should only be operated in a controlled 'concrete lake' environment  

For support queries regarding your SD Twizy: [support@streetdrone.zendesk.com](support@streetdrone.zendesk.com)

#### In this release (beta):
* Message passing and parsing from Autoware to the vehicle's CAN utilising the SocketCAN open source library and networking stack
* Continuous software to Drive-by-Wire (DBW) system handshake verification
* Manual to autonomous hand-over in conjunction with safety driver physical interaction with the vehicle HMI
* Vehicle status monitoring
* Conversion of the `twist_cmd` linear velocity and angular acceleration target into low level throttle/brake/steer commands
* Tuned PID Controller for the vehicle's speed

sd_twizy_interface: The vehicle interface node for the StreetDrone (SD) Twizy. This node translates the output messages from Autoware (geometry_msgs::TwistStamped) to SocketCAN messages (can_msgs::Frame) and vice versa. The node integrates a twist to ackermann function for controlling the steering of the vehicle and a PID controller with vehicle speed feedback corresponding to the output twist messages from Autoware.  

socketcan_bridge: This node comes from the default ROS package [socketcan_bridge](http://wiki.ros.org/socketcan_bridge). The package provides functionality to expose CAN frames from SocketCAN to a ROS Topic. Internally it uses the socketcan_interface from the ros_canopen package, as such, it is capable of dealing with both normal and extended CAN frames. 

#### Limitations:
* Speed control with closed loop feedback, limited to 10 mps
* Negative velocity requests are interpreted as a target of 0 mps instead of reverse
* Angular speed requests without feedback loop, limited to 100 deg/s
* Accuracy of the controller may vary dependant on the environmental conditions at the time of test

## Requirements

##### - Ubuntu 16.04 LTS
##### - ROS Kinetic [ros-kinetic-desktop-full](http://wiki.ros.org/kinetic/Installation/Ubuntu)
##### - Catkin Command Line Tools [catkin_tools](https://catkin-tools.readthedocs.io/en/latest/installing.html)


## How to build and use the `sd` package in Autoware
1. Open terminal and clone [Autoware](https://github.com/autowarefoundation/autoware) with `git clone`

2. Then get all the submodules
```
cd autoware
git submodule update --init --recursive
```

3. Install the ROS packages: [socketcan_interface](http://wiki.ros.org/socketcan_interface) and [can_msgs](http://wiki.ros.org/can_msgs)
```
sudo apt-get install ros-kinetic-socketcan-interface ros-kinetic-can-msgs
```


5. Go to the `/src` folder of Autoware's catkin workspace with `cd /ros/src` and clone the sd vehicle interface package there with `git clone`

6. Build the package
 ```
cd ../
catkin build sd
```

If you previously built your workspace with `catkin_make`, either clean your workspace with `catkin clean` (if you know what you are doing) and rebuild with `catkin build sd` OR don't clean your workspace and simply do `catkin_make --pkg sd`.  
On this step you might get the error: `Could not find a package configuration file provided by "jsk_recognition_msgs"`  
To fix the issue simply run `sudo apt-get install ros-kinetic-jsk-recognition-msgs`


5. Source your setup.*sh file:
```
source devel/setup.bash
```

6. Once connected to the vehicle, to establish the CAN communication and launch the node, a script is provided for convenience:
```
cd /src/SD-VehicleInterface/sd
./sd_twizy_interface.sh
```
If the bash script isn't running, do:
```
chmod a+wx sd_twizy_interface.sh
```

If you prefer not to use the script, simply do:
```
sudo modprobe peak_usb

sudo ip link set can0 type can bitrate 500000
sudo ip link set can0 up

cd autoware/ros
source devel/setup.bash
roslaunch sd sd_twizy_interface.launch
```


