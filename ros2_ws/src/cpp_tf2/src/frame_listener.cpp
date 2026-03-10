#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

class FrameListener : public rclcpp::Node {
public:
    FrameListener() : Node("frame_listener"), buffer_(this->get_clock()), listener_(buffer_) {
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1), 
            std::bind(&FrameListener::lookupTransform, this)
        );
    }

private:
    void lookupTransform() {
        try {
            auto transform = buffer_.lookupTransform("world", "robot", tf2::TimePointZero);
            RCLCPP_INFO(this->get_logger(), "Transform: x=%f, y=%f, z=%f", 
                        transform.transform.translation.x, 
                        transform.transform.translation.y, 
                        transform.transform.translation.z);
        } catch (const tf2::TransformException &ex) {
            RCLCPP_WARN(this->get_logger(), "Failed to lookup transform: %s", ex.what());
        }
    }

    tf2_ros::Buffer buffer_;
    tf2_ros::TransformListener listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FrameListener>());
    rclcpp::shutdown();
    return 0;
}
