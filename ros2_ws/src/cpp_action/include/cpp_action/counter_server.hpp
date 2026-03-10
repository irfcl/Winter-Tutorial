#ifndef COUNTER_CLIENT_HPP_
#define COUNTER_CLIENT_HPP_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include "rcl_action/action_server.h"
#include "example_interfaces/action/counter.hpp"
#include "std_msgs/msg/int32.hpp"

class CounterActionServer : public rclcpp::Node 
{
public:
    CounterActionServer(const rclcpp::NodeOptions & options) 
    : Node("counter_server", options) {
        // use Reentrant and create a callback group
        callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        this->action_server_ = rclcpp_action::create_server<example_interfaces::action::Counter>(
            this,
            "count_to",
            std::bind(&CounterActionServer::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
            std::bind(&CounterActionServer::handle_cancel, this, std::placeholders::_1),
            std::bind(&CounterActionServer::handle_accepted, this, std::placeholders::_1),
            rcl_action_server_get_default_options(),
            callback_group_                         
        );
    }

private:
    double start_time, current_time;
    int current_number;
    rclcpp_action::Server<example_interfaces::action::Counter>::SharedPtr action_server_;
    rclcpp::CallbackGroup::SharedPtr callback_group_;

    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID & uuid,
        std::shared_ptr<const example_interfaces::action::Counter::Goal> goal);
    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<rclcpp_action::ServerGoalHandle<example_interfaces::action::Counter>> goal_handle);
    void handle_accepted(const std::shared_ptr<rclcpp_action::ServerGoalHandle<example_interfaces::action::Counter>> goal_handle);
};

class PrimeFinderSubscriber : public rclcpp::Node 
{
public:
    PrimeFinderSubscriber(const rclcpp::NodeOptions & options) : Node("prime_finder", options) {
        auto cb_group = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        auto sub_options = rclcpp::SubscriptionOptions();
        sub_options.callback_group = cb_group;

        subscription_ = this->create_subscription<std_msgs::msg::Int32>(
            "target_num", 10, std::bind(&PrimeFinderSubscriber::calculate_primes, this, std::placeholders::_1));
    }

private:
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;

    void calculate_primes(const std_msgs::msg::Int32::SharedPtr msg);
};

#endif