#include <rclcpp/rclcpp.hpp>
#include <turtlesim/msg/pose.hpp>
#include <turtlesim/srv/spawn.hpp>
#include <turtlesim/srv/kill.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <cmath>
 
class TurtleChase : public rclcpp::Node
{
public:
   TurtleChase() : Node("turtle_chase")
   {
       // Spawn turtles
       spawn_turtle("A", 5.0, 5.0);
       spawn_turtle("B", rand() % 10, rand() % 10);
 
       // Create a publisher for turtle A
       cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("A/cmd_vel", 10);
 
       // Create a subscription for turtle B's position
       b_pose_sub_ = this->create_subscription<turtlesim::msg::Pose>(
           "B/pose", 10, std::bind(&TurtleChase::b_pose_callback, this, std::placeholders::_1));
 
       // Timer to update A's position
       timer_ = this->create_wall_timer(
           std::chrono::milliseconds(100), std::bind(&TurtleChase::move_turtle_a, this));
   }
 
private:
   void spawn_turtle(const std::string &name, float x, float y)
{
    auto client = this->create_client<turtlesim::srv::Spawn>("spawn");
    while (!client->wait_for_service(std::chrono::seconds(1))) {
        RCLCPP_INFO(this->get_logger(), "Waiting for service to spawn turtle...");
    }

    auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
    request->name = name;
    request->x = x;
    request->y = y;
    request->theta = 0;

    auto future = client->async_send_request(request);
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
        rclcpp::FutureReturnCode::SUCCESS) {
        RCLCPP_INFO(this->get_logger(), "Spawned turtle: %s", name.c_str());
    }
}
   void b_pose_callback(const turtlesim::msg::Pose::SharedPtr msg)
   {
       b_x_ = msg->x;
       b_y_ = msg->y;
   }
 
   void move_turtle_a()
   {
       auto msg = geometry_msgs::msg::Twist();
       float distance = std::sqrt(std::pow(b_x_ - a_x_, 2) + std::pow(b_y_ - a_y_, 2));
       if (distance > 1.0) {
           msg.linear.x = 1.0;
           msg.angular.z = std::atan2(b_y_ - a_y_, b_x_ - a_x_);
       } else {
           msg.linear.x = 0.0;
           msg.angular.z = 0.0;
       }
       cmd_pub_->publish(msg);
   }
 
   rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
   rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr b_pose_sub_;
   rclcpp::TimerBase::SharedPtr timer_;
 
   float a_x_ = 5.0, a_y_ = 5.0; // A turtle's position
   float b_x_, b_y_; // B turtle's position
};
 
int main(int argc, char **argv)
{
   rclcpp::init(argc, argv);
   rclcpp::spin(std::make_shared<TurtleChase>());
   rclcpp::shutdown();
   return 0;
}