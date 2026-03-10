#ifndef COUNTER_SERVER_HPP_
#define COUNTER_SERVER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/counter.hpp"
#include "std_msgs/msg/int32.hpp"

class CounterActionClient : public rclcpp::Node
{
public:
    explicit CounterActionClient(const rclcpp::NodeOptions & options)
    : Node("example_action_client", options) {
        this->action_client_ = rclcpp_action::create_client<example_interfaces::action::Counter>(
        this, "count_to");

        this->timer_ = this->create_wall_timer(
        std::chrono::milliseconds(500),
        std::bind(&CounterActionClient::send_goal, this));
    }

    void send_goal();

private:
    rclcpp_action::Client<example_interfaces::action::Counter>::SharedPtr action_client_;
    rclcpp::TimerBase::SharedPtr timer_;

    void goal_response_callback(std::shared_future<rclcpp_action::ClientGoalHandle
        <example_interfaces::action::Counter>::SharedPtr> future);

    void feedback_callback(
        rclcpp_action::ClientGoalHandle<example_interfaces::action::Counter>::SharedPtr,
        const std::shared_ptr<const example_interfaces::action::Counter::Feedback> feedback);

    void result_callback(const rclcpp_action::ClientGoalHandle
        <example_interfaces::action::Counter>::WrappedResult & result);
};

#endif