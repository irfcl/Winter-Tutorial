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

    // 啟動所有執行緒
    void run() {
        // 分別以 0, 10, 20, 30 ms 的延遲啟動 4 個執行緒
        for (int i = 0; i < 4; ++i) {
            workers_.emplace_back(&MultiThreadedTimer::worker_task, this, i * 10ms);
        }
    }

    // 解構時確保資源正確釋放 (RAII)
    ~MultiThreadedTimer() {
        stop();
    }

    void stop() {
        running_ = false; // 原子操作，確保所有執行緒同步收到停止指令
        for (auto& t : workers_) {
            if (t.joinable()) {
                t.join(); // 等待執行緒結束，避免程式崩潰
            }
        }
    }

private:
    // 模擬 EventQueue 的定時執行邏輯
    void worker_task(std::chrono::milliseconds initial_offset) {
        std::this_thread::sleep_for(initial_offset); // 初始交錯延遲

        while (running_) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_);

            // 設定停止條件：運行超過 10,000 ms (10秒)
            if (elapsed.count() > 10000) break;

            safe_print(elapsed.count());

            // 模擬 call_every(40ms) 的行為
            std::this_thread::sleep_for(40ms);
        }
    }

    // 解決 Race Condition：使用 Mutex 保護標準輸出流
    void safe_print(long long ms) {
        std::lock_guard<std::mutex> lock(print_mtx_);
        std::cout << "[Thread " << std::this_thread::get_id() << "] Elapsed: " 
                  << ms << " ms" << std::endl;
    }

    std::atomic<bool> running_;              // 執行緒安全的開關
    std::chrono::steady_clock::time_point start_time_;
    std::mutex print_mtx_;                   // 防止輸出字元交錯的鎖
    std::vector<std::thread> workers_;       // 管理複數個執行緒
};

int main() {
    std::cout << "系統啟動：開始多執行緒並行任務..." << std::endl;
    
    MultiThreadedTimer timer_app;
    timer_app.run();

    // 主執行緒可以執行其他任務，或簡單等待
    std::this_thread::sleep_for(11s); 

    std::cout << "所有任務已完成。" << std::endl;
    return 0;
}