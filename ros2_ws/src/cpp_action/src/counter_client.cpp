#include "cpp_action/counter_client.hpp"

void CounterActionClient::send_goal()
{
    this->timer_->cancel();

    auto message = std_msgs::msg::Int32();
    message.data = 100000;
    RCLCPP_INFO(this->get_logger(), "Publishing: %d", message.data);
    publisher_->publish(message);

    if (!this->action_client_->wait_for_action_server()) {
        RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
        rclcpp::shutdown();
    }

    auto goal_msg = example_interfaces::action::Counter::Goal();
    goal_msg.target_number = 100000;

    RCLCPP_INFO(this->get_logger(), "Sending goal");

    auto send_goal_options = rclcpp_action::Client<example_interfaces::action::Counter>::SendGoalOptions();
    // send_goal_options.goal_response_callback = 
    // std::bind(&CounterActionClient::goal_response_callback, this, std::placeholders::_1);
    send_goal_options.feedback_callback =
    std::bind(&CounterActionClient::feedback_callback, this, std::placeholders::_1, std::placeholders::_2);
    send_goal_options.result_callback =
    std::bind(&CounterActionClient::result_callback, this, std::placeholders::_1);
    this->action_client_->async_send_goal(goal_msg, send_goal_options);
}

void CounterActionClient::goal_response_callback(std::shared_future<rclcpp_action::ClientGoalHandle
    <example_interfaces::action::Counter>::SharedPtr> future) {
    auto goal_handle = future.get();
    if (!goal_handle) {
        RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    } else {
        RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
    }
}

void CounterActionClient::feedback_callback(
    rclcpp_action::ClientGoalHandle<example_interfaces::action::Counter>::SharedPtr,
    const std::shared_ptr<const example_interfaces::action::Counter::Feedback> feedback) {
    RCLCPP_INFO_STREAM(this->get_logger(), "Feedback get! current number is" << feedback->current_number);
}

void CounterActionClient::result_callback(const rclcpp_action::ClientGoalHandle
    <example_interfaces::action::Counter>::WrappedResult & result) {
    switch (result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:
        break;
    case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(this->get_logger(), "Goal was aborted");
        return;
    case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_ERROR(this->get_logger(), "Goal was canceled");
        return;
    default:
        RCLCPP_ERROR(this->get_logger(), "Unknown result code");
        return;
    }
    RCLCPP_INFO_STREAM(this->get_logger(), "Result get! Spend" << result.result->time_spent << " second.");
    rclcpp::shutdown();
}