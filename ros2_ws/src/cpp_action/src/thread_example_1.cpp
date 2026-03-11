#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <atomic>

using namespace std::chrono_literals;

class MultiThreadedTimer {
public:
    MultiThreadedTimer() : running_(true) {
        start_time_ = std::chrono::steady_clock::now();
    }

    void run() {
        workers_.emplace_back(&MultiThreadedTimer::worker_task, this, 0ms);
    }

    ~MultiThreadedTimer() {
        stop();
    }

    void stop() {
        running_ = false;
        for (auto& t : workers_) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

private:
    // EventQueue
    void worker_task(std::chrono::milliseconds initial_offset) {
        std::this_thread::sleep_for(initial_offset);

        while (running_) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_);

            if (elapsed.count() > 10000) break;
            safe_print(elapsed.count());
            std::this_thread::sleep_for(1ms);
        }
    }

    // tackle Race Condition
    void safe_print(long long ms) {
        std::lock_guard<std::mutex> lock(print_mtx_);
        std::cout << "[Thread " << std::this_thread::get_id() << "] Elapsed: " 
                  << ms << " ms" << std::endl;
    }

    std::atomic<bool> running_;
    std::chrono::steady_clock::time_point start_time_;
    std::mutex print_mtx_;
    std::vector<std::thread> workers_;
};

int main() {
    MultiThreadedTimer timer_app;
    timer_app.run();

    std::this_thread::sleep_for(11s); 
    return 0;
}