#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

class PointTransformer : public rclcpp::Node {
public:
    PointTransformer() : Node("point_transformer"), buffer_(this->get_clock()), listener_(buffer_) {
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1), 
            std::bind(&PointTransformer::transformPoint, this)
        );
    }

private:
    void transformPoint() {
        geometry_msgs::msg::PointStamped point;
        point.header.frame_id = "robot";
        point.header.stamp = this->get_clock()->now();
        point.point.x = 1.0;
        point.point.y = 2.0;
        point.point.z = 3.0;

        try {
            auto transformed_point = buffer_.transform(point, "world");
            RCLCPP_INFO(this->get_logger(), "Transformed Point: x=%f, y=%f, z=%f",
                        transformed_point.point.x, 
                        transformed_point.point.y, 
                        transformed_point.point.z);
        } catch (const tf2::TransformException &ex) {
            RCLCPP_WARN(this->get_logger(), "Failed to transform point: %s", ex.what());
        }
    }

    tf2_ros::Buffer buffer_;
    tf2_ros::TransformListener listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PointTransformer>());
    rclcpp::shutdown();
    return 0;
}
