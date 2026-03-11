#include "cpp_action/counter_server.hpp"

rclcpp_action::GoalResponse CounterActionServer::handle_goal(
    const rclcpp_action::GoalUUID & uuid,
    std::shared_ptr<const example_interfaces::action::Counter::Goal> goal) {
    RCLCPP_INFO(this->get_logger(), "Received goal request with target number %d", goal->target_number);
    (void)uuid;
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse CounterActionServer::handle_cancel(
    const std::shared_ptr<rclcpp_action::ServerGoalHandle<example_interfaces::action::Counter>> goal_handle) {
    RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
}

void CounterActionServer::handle_accepted(const std::shared_ptr
    <rclcpp_action::ServerGoalHandle
    <example_interfaces::action::Counter>> goal_handle) {
    using namespace std::placeholders;
    rclcpp::Rate loop_rate(100);
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<example_interfaces::action::Counter::Feedback>();
    auto result = std::make_shared<example_interfaces::action::Counter::Result>();
    int i;
    this->start_time = this->get_clock()->now().seconds();

    for (i = 0; i < goal->target_number; ++i) {
        this->current_time = this->get_clock()->now().seconds();
        // Check if there is a cancel request
        if (goal_handle->is_canceling()) {
            result->time_spent = this->current_time - this->start_time;
            goal_handle->canceled(result);
            RCLCPP_INFO(this->get_logger(), "Goal canceled");
            return;
        }
        // Update feedback
        feedback->current_number = i;
        goal_handle->publish_feedback(feedback);
        RCLCPP_INFO(this->get_logger(), "Publish feedback");
        loop_rate.sleep();
    }
    // Check if goal is done
    if (rclcpp::ok()) {
        result->time_spent = this->current_time - this->start_time;
        goal_handle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Goal succeeded");
    }
}

void PrimeFinderSubscriber::calculate_primes(const std_msgs::msg::Int32::SharedPtr msg) {
    int target_number = msg->data;
    RCLCPP_INFO(this->get_logger(), "start counting the number of the prime number under %d", target_number);
    int count = 0;
    for (int i = 2; i <= target_number; i++) {
        bool is_prime = true;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) { 
                is_prime = false; 
                break; 
            }
        }
        if (is_prime) 
            count++;
    }
    RCLCPP_INFO(this->get_logger(), "finish! has %d prime numbers", count);
}