#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include "example_interfaces/action/Counter.hpp"

#include "cpp_action/counter_client.hpp"
#include "cpp_action/counter_server.hpp"

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);

    // Intra-process Communication
    rclcpp::NodeOptions options;
    options.use_intra_process_comms(true);

    auto action_client = std::make_shared<CounterActionClient>(options);
    auto action_server = std::make_shared<CounterActionServer>(options);
    auto prime_finder = std::make_shared<PrimeFinderSubscriber>(options);

    // create multi-threaded executor
    rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 4);

    // add all nodes into one executor
    executor.add_node(action_client);
    executor.add_node(action_server);
    executor.add_node(prime_finder);

    executor.spin();
    rclcpp::shutdown();
    return 0;
}