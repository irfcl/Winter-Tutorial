#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

class FrameBroadcaster : public rclcpp::Node {
public:
    FrameBroadcaster() : Node("frame_broadcaster") {
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100), 
            std::bind(&FrameBroadcaster::broadcastTransform, this)
        );
    }

private:
    void broadcastTransform() {
        geometry_msgs::msg::TransformStamped transform;
        transform.header.stamp = this->get_clock()->now();
        transform.header.frame_id = "world";
        transform.child_frame_id = "robot";

        transform.transform.translation.x = 1.0;
        transform.transform.translation.y = 2.0;
        transform.transform.translation.z = 0.0;
        transform.transform.rotation.x = 0.0;
        transform.transform.rotation.y = 0.0;
        transform.transform.rotation.z = 0.7071;
        transform.transform.rotation.w = 0.7071;

        broadcaster_->sendTransform(transform);
    }

    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FrameBroadcaster>());
    rclcpp::shutdown();
    return 0;
}
