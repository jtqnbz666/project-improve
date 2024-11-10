// #include <iostream>
// #include <chrono>
// #include <thread>
// #include <mutex>

// class Snowflake {
// public:
//     static void initialize(int machine_id) {
//         if (machine_id < 0 || machine_id >= max_machine_id) {
//             throw std::invalid_argument("Machine ID is out of range");
//         }
//         machine_id_ = machine_id;
//         epoch_ = std::chrono::system_clock::now().time_since_epoch().count() - start_time;
//     }

//     static uint64_t nextId() {
//         std::lock_guard<std::mutex> lock(mutex_);

//         auto now = std::chrono::system_clock::now().time_since_epoch().count() - start_time;
//         if (now < last_timestamp_) {
//             throw std::runtime_error("Clock moved backwards, refusing to generate id");
//         }

//         if (now == last_timestamp_) {
//             sequence_ = (sequence_ + 1) & sequence_mask;
//             if (sequence_ == 0) {
//                 now = waitNextMillis(now);
//             }
//         } else {
//             sequence_ = 0;
//         }

//         last_timestamp_ = now;
//         return ((now - epoch_) << timestamp_left_shift) |
//                (machine_id_ << machine_id_shift) |
//                sequence_;
//     }

// private:
//     static int machine_id_;
//     static int64_t last_timestamp_;
//     static int64_t sequence_;
//     static int64_t epoch_;

//     static const int64_t start_time = 1609459200000; // 2021-01-01
//     static const int64_t machine_id_bits = 10;
//     static const int64_t max_machine_id = (1 << machine_id_bits) - 1;
//     static const int64_t sequence_bits = 12;
//     static const int64_t machine_id_shift = sequence_bits;
//     static const int64_t timestamp_left_shift = sequence_bits + machine_id_bits;
//     static const int64_t sequence_mask = (1 << sequence_bits) - 1;

//     static int64_t waitNextMillis(int64_t last_timestamp) {
//         int64_t timestamp = std::chrono::system_clock::now().time_since_epoch().count() - start_time;
//         while (timestamp <= last_timestamp) {
//             timestamp = std::chrono::system_clock::now().time_since_epoch().count() - start_time;
//         }
//         return timestamp;
//     }

//     static std::mutex mutex_;
// };

// int Snowflake::machine_id_ = 0;
// int64_t Snowflake::last_timestamp_ = 0;
// int64_t Snowflake::sequence_ = 0;
// int64_t Snowflake::epoch_ = 0;
// std::mutex Snowflake::mutex_;

