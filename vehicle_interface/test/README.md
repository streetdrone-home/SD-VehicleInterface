# Vehicle Interface Unit Testing


## Levels of Testing in ROS http://wiki.ros.org/Quality/Tutorials/UnitTesting
There are 4 levels for tests in ROS
Level 1: gtest and unittes only
https://github.com/google/googletest/tree/master/googletest/samples 10 samples for gtest
Level 2: Level1 + ROSTEST for testing publishers, subscribers, parameters http://wiki.ros.org/rostest/Nodes
https://github.com/steup/Ros-Test-Example/blob/ede8186d7bdffd8cbc8fbaed266e6845861dc75c/src/cars/src/Test.cpp Sample for a node testing
Level 3: Testing multiple nodes with test cases and using ROSBAGS if needed 
https://answers.ros.org/question/195054/how-to-verify-publications-using-rostest/ has sample of a simple publisher subscriber testing
https://github.com/strawlab/image_pipeline/blob/master/image_proc/test/rostest.cpp Another sample
Level 4: Testing in simulation (AWS RoboMaker with Gazebo)

## Create Unit Tests
To add unit tests to package please follow these steps: http://wiki.ros.org/rostest/Writing
1- Add test folder inside the package at the same level of src folder
2- Add catkin lines in CMakeLists to launch unit test. Look in (http://docs.ros.org/kinetic/api/catkin/html/howto/format2/rostest_configuration.html)
3- In Launch folder add either a .test or .launch xml files with </test> tag
4- Add .cpp (gtest) or .py (unittest) file for testing
5- Make sure to cover all test cases for each function with correct test tag https://stackoverflow.com/questions/58600728/what-is-the-difference-between-test-test-f-and-test-p
6- Run the test

## Build and Run Unit Tests http://wiki.ros.org/rostest/Commandline and http://docs.ros.org/kinetic/api/catkin/html/howto/format2/run_tests.html
To run all unit tests
```
catkin_make && catkin_make run_tests
```
OR for specific test
```
rostest --text <pkg_name> <test_file>.test
```

## Redesign node for unit testing http://wiki.ros.org/CppStyleGuide
1- Make sure the node has a separate main file than the file with functions and variables definition
2- Usage of structs, classes are preferred in creating functionalities
3- RUN function in each class for each node is used to define subscribers, publishers, parameters and main pipeline of the node
4- Each function is preffered to have a return even if int or string to help in unit testing and debugging
5- Each funtion has to be well defined so it is responsible for only one act and has return in all its branches whether if conditions or any other branching technique