# 1. Self Drivring Car
Source code C++ on Ubuntu (FPT Digital Race 2019)
# 2. Installation
Requirements:
- OS Ubuntu 16.04 (or 18.04)
- OpenCV 3.4.2
- ROS (Distro: Melodic or Lunar)
- C++ 11.0
- CMake

To install this package, open terminal and enter:

`git clone: https://github.com/haimeohung/Self_Drivring_Car.git`

To build, type: 

`mkdir catkin_ws/build`

`cd catkin_ws`

`catkin_make`

Copy folder from [your_clone_respository]/src into catkin_ws/src, then type `catkin_make` again.

# 3. Usage
Initialize ros-master: 

`roslaunch team305 team305.launch`

Now you can connect to a simulator software by fill team_name and ip address.

For example: team305 - ws://127.0.0.1:9005

If you don't like my team's name or want to connect by another address, you can edit node_name and file_server tag in team305. launch file.

# 4. Simulation material
Get in URL: https://drive.google.com/open?id=1wI5_UWDXUNwNHWy-g303rsd7nVxGXRzy 
# 5. Content

https://drive.google.com/open?id=1rwI-2c32rM2McZJgayUS8J_p-5ZOTQgV

# 6. Test video

https://youtu.be/9e7tzggmux0
