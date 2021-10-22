#include <ros/ros.h>
#include <gtest/gtest.h>
#include "sd_vehicle_interface.h"

sd_vehicle_interface sd_vi;

class TargetTest: public ::testing::Test
{
public:
    TargetTest(): spinner(0) {};
    ~TargetTest() {};

    ros::NodeHandle* node;
    ros::AsyncSpinner* spinner;

    void SetUp() override
    {
        ::testing::Test::SetUp();
        this->node = new ros::NodeHandle("~");
        this->spinner = new ros::AsyncSpinner(0);
        this->spinner->start();
    };

    void TearDown() override
    {
        ros::shutdown();
        delete this->spinner;
        delete this->node;
        ::testing::Test::TearDown();
    }
};

TEST_F(TargetTest, test_ok)
{
    ASSERT_TRUE(true); // Make sure the node is up and running
}

TEST (SPEEDSourceTest, speed_source)
{
    // Check if output of speed source function is equivalent to the expected outputs
    EXPECT_EQ(sd_vi.CurrentTwistLinearNDT_Mps, sd_vi.speedSource("ndt_speed"));
    EXPECT_EQ(sd_vi.CurrentTwistLinearCANImu_Mps, sd_vi.speedSource("imu_speed"));
    EXPECT_EQ(sd_vi.CurrentTwistLinearCANSD_Mps, sd_vi.speedSource("vehicle_can_speed"));
    EXPECT_EQ(0, sd_vi.speedSource("aslan_speed"));
}

TEST(TwistTest, twist_setup_test)
{
    // This is a test for pass by reference and pointers unit test
    geometry_msgs::TwistStamped test_Twist;
    sd_vi.Twist_setup(&test_Twist);
    EXPECT_EQ(sd_vi.IMU_Rate_Z * DEG_to_RAD, test_Twist.twist.angular.z);
    EXPECT_EQ(sd_vi.CurrentTwistLinearSD_Mps_Final, test_Twist.twist.linear.z);
}

TEST(GPSTest, gps_setup_test)
{
    // This is a work around if pass by reference was not needed
    sensor_msgs::NavSatFix test_GPS;
    test_GPS = sd_vi.GPS_setup(test_GPS);
    EXPECT_EQ(test_GPS.longitude, 0.0);
    EXPECT_EQ(test_GPS.latitude, sd_vi.GPS_Latitude);
}

TEST(ControlTest, AutonomousControl_setup_test)
{
    // Ensuring all function has return helps in unit testing such functions even if return is not used
    sd_vi.AutomationArmed_B = true;
    ASSERT_TRUE(sd_vi.AutonomousControl_setup() == "Autonomous");
    sd_vi.AutomationArmed_B = false;
    ASSERT_TRUE(sd_vi.AutonomousControl_setup() == "Manual");
    sd_vi.AliveCounter_Z = 1;
    ASSERT_TRUE(sd_vi.AutonomousControl_setup() == "FREQUENCY_ERROR");
}

TEST(AutonomousTest, AutonomousControl_action_setup_test)
{
    // Make sure to input all input variables as previous test cases might change values
    aslan_msgs::SDControl SD_Test_Control;
    sd_vi.AutomationGranted_B = true;
    sd_vi._sd_simulation_mode = false;
    ASSERT_TRUE(sd_vi.AutonomousControl_action(&SD_Test_Control) == "Autonomous");
    sd_vi.AutomationGranted_B = false;
    sd_vi._sd_simulation_mode = true;
    ASSERT_TRUE(sd_vi.AutonomousControl_action(&SD_Test_Control) == "Autonomous");
    sd_vi.AutomationGranted_B = false;
    sd_vi._sd_simulation_mode = false;
    ASSERT_TRUE(sd_vi.AutonomousControl_action(&SD_Test_Control) == "Manual");
}

TEST(SpeedTest, speed_control_test)
{
    aslan_msgs::SDControl SD_test_Control;
    sd_vi.AliveCounter_Z = 200;    
    ros::NodeHandle nh;
    sd_vi.sd_control_pub = nh.advertise<aslan_msgs::SDControl>("sd_control", 1, true);
    ASSERT_TRUE("SPEED_DONE" == sd_vi.SpeedControl(&SD_test_Control)); // No topic to publish at if line is commented and sd_vi.RUN is not called, an error will happen
    sd_vi.AliveCounter_Z = 1;
    ASSERT_TRUE("FREQUENCY_ERROR" == sd_vi.SpeedControl(&SD_test_Control));
}

TEST(VehicleTest, choose_vehicle_test)
{
    sd_vi._sd_vehicle = "twizy";
    ASSERT_TRUE("Twizy" == sd_vi.ChooseVehicle())<< "Car chosen is not twizy"; // To output message witht the error
    sd_vi._sd_vehicle = "this is error";
    ASSERT_TRUE("Env200" == sd_vi.ChooseVehicle());
}

int main(int argc, char **argv)
{
    // This should be the only main either in the code tested or all the files of unit tests run by this main
    ros::init(argc, argv, "vi_test");
    if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug))
    {
        ros::console::notifyLoggerLevelsChanged(); // To show debug output in the tests
    }
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}