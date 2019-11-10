//
// Created by zeke on 11/10/19.
//

#ifndef AUDIOVIS_DATAQUEUE_H
#define AUDIOVIS_DATAQUEUE_H

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

///
/// \class DataQueue
///
/// \brief Synchronized Queue that stores Displayable Data
class DataQueue {

public:

    ///
    /// \brief enqueues data
    /// \param[in] data item to enqueue
    void enqueue(std::vector<double>& data) {
        {
            std::unique_lock<std::mutex> lockGuard (_mutex);

            _queue.push(data);
        }
        _condition.notify_all();
    }

    ///
    /// \brief  dequeues data, returning an empty optional if wait
    /// timed out
    /// \return optional wrapping displayable data
    std::optional<std::vector<double>> dequeue() {
        std::unique_lock<std::mutex> lockGuard (_mutex);

        using namespace std::chrono_literals;
        _condition.wait_for(lockGuard, 1s, [&] { return !_queue.empty(); });

        if (!_queue.empty()) {
            std::vector<double> data = _queue.front();
            _queue.pop();
            return data;
        } else {
            return {}; // empty optional to indicate queue is empty after waiting
        }
    }

private:

    std::queue<std::vector<double>> _queue;

    std::mutex _mutex;

    std::condition_variable _condition;

};


#endif //AUDIOVIS_DATAQUEUE_H
