#ifndef _EFP_CONCURRENCY_TEST_HPP
#define _EFP_CONCURRENCY_TEST_HPP

#include "catch2/catch_test_macros.hpp"

#include "mth.hpp"

// todo Theading in Catch2 in unstable at the moment and aborting time to time. Though it passes if get run

TEST_CASE("Enqueue and Dequeue", "[BlockingQ]") {
    BlockingQ<int, 5> queue;

    SECTION("Enqueue then Dequeue") {
        queue.enqueue(1);
        CHECK(queue.dequeue() == 1);
    }

    SECTION("Multiple Enqueue and Dequeue") {
        queue.enqueue(1);
        queue.enqueue(2);
        CHECK(queue.dequeue() == 1);
        CHECK(queue.dequeue() == 2);
    }
}

TEST_CASE("Enqueue and Dequeue Dynamic Capacity", "[BlockingQ]") {
    BlockingQ<int> queue;

    SECTION("Enqueue then Dequeue") {
        queue.enqueue(1);
        CHECK(queue.dequeue() == 1);
    }

    SECTION("Multiple Enqueue and Dequeue") {
        queue.enqueue(1);
        queue.enqueue(2);
        CHECK(queue.dequeue() == 1);
        CHECK(queue.dequeue() == 2);
    }
}

// TEST_CASE("Blocking Behavior", "[BlockingQ]") {
//     BlockingQ<int, 5> queue;

//     SECTION("Dequeue Blocks When Queue is Empty") {
//         std::thread t([&]() {
//             std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Delay to ensure dequeue blocks
//             queue.enqueue(1);
//         });

//         auto start = std::chrono::high_resolution_clock::now();
//         CHECK(queue.dequeue() == 1); // This should block until the other thread enqueues
//         auto end = std::chrono::high_resolution_clock::now();
//         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

//         CHECK(duration.count() >= 100); // Check if there was a block
//         t.join();
//     }
// }

// TEST_CASE("Blocking Behavior Dynamic Capacity", "[BlockingQ]") {
//     BlockingQ<int> queue;

//     SECTION("Dequeue Blocks When Queue is Empty") {
//         std::thread t([&]() {
//             std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Delay to ensure dequeue blocks
//             queue.enqueue(1);
//         });

//         auto start = std::chrono::high_resolution_clock::now();
//         CHECK(queue.dequeue() == 1); // This should block until the other thread enqueues
//         auto end = std::chrono::high_resolution_clock::now();
//         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

//         CHECK(duration.count() >= 100); // Check if there was a block
//         t.join();
//     }
// }

// TEST_CASE("Thread Safety", "[BlockingQ]") {
//     BlockingQ<int, 5> queue;
//     efp::Vector<std::thread> threads;

//     for (int i = 0; i < 10; ++i) {
//         threads.emplace_back([&queue, i]() {
//             queue.enqueue(i);
//         });
//     }

//     efp::Vector<int> results;
//     for (int i = 0; i < 10; ++i) {
//         threads.emplace_back([&queue, &results]() {
//             results.push_back(queue.dequeue());
//         });
//     }

//     for (auto& t : threads) {
//         t.join();
//     }

//     CHECK(results.size() == 10); // Ensure we dequeued 10 items
//     // Further checks can be added to verify the dequeued values
// }

// TEST_CASE("Thread Safety Dynamic Capacity", "[BlockingQ]") {
//     BlockingQ<int> queue;
//     efp::Vector<std::thread> threads;

//     for (int i = 0; i < 10; ++i) {
//         threads.emplace_back([&queue, i]() {
//             queue.enqueue(i);
//         });
//     }

//     efp::Vector<int> results;
//     for (int i = 0; i < 10; ++i) {
//         threads.emplace_back([&queue, &results]() {
//             results.push_back(queue.dequeue());
//         });
//     }

//     for (auto& t : threads) {
//         t.join();
//     }

//     CHECK(results.size() == 10); // Ensure we dequeued 10 items
//     // Further checks can be added to verify the dequeued values
// }

TEST_CASE("Enqueue and Dequeue", "[NonBlockingQ]") {
    NonBlockingQ<int, 5> queue;

    SECTION("Enqueue then Dequeue") {
        queue.enqueue(1);
        auto result = queue.dequeue();
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 1);
    }

    SECTION("Dequeue from Empty Queue") {
        auto result = queue.dequeue();
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Multiple Enqueue and Dequeue") {
        queue.enqueue(1);
        queue.enqueue(2);
        REQUIRE(queue.dequeue().value() == 1);
        REQUIRE(queue.dequeue().value() == 2);
    }
}

TEST_CASE("Enqueue and Dequeue Dynamic Capacity", "[NonBlockingQ]") {
    NonBlockingQ<int> queue;

    SECTION("Enqueue then Dequeue") {
        queue.enqueue(1);
        auto result = queue.dequeue();
        REQUIRE(result.has_value());
        REQUIRE(result.value() == 1);
    }

    SECTION("Dequeue from Empty Queue") {
        auto result = queue.dequeue();
        REQUIRE_FALSE(result.has_value());
    }

    SECTION("Multiple Enqueue and Dequeue") {
        queue.enqueue(1);
        queue.enqueue(2);
        REQUIRE(queue.dequeue().value() == 1);
        REQUIRE(queue.dequeue().value() == 2);
    }
}

TEST_CASE("Empty State", "[NonBlockingQ]") {
    NonBlockingQ<int, 5> queue;

    SECTION("Initially Empty") {
        REQUIRE(queue.is_empty());
    }

    SECTION("NotEmpty After Enqueue") {
        queue.enqueue(1);
        REQUIRE_FALSE(queue.is_empty());
    }

    SECTION("Empty After Dequeuing Last Element") {
        queue.enqueue(1);
        queue.dequeue();
        REQUIRE(queue.is_empty());
    }
}

TEST_CASE("Empty State Dynamic Capacity", "[NonBlockingQ]") {
    NonBlockingQ<int> queue;

    SECTION("Initially Empty") {
        REQUIRE(queue.is_empty());
    }

    SECTION("NotEmpty After Enqueue") {
        queue.enqueue(1);
        REQUIRE_FALSE(queue.is_empty());
    }

    SECTION("Empty After Dequeuing Last Element") {
        queue.enqueue(1);
        queue.dequeue();
        REQUIRE(queue.is_empty());
    }
}

#endif // _EFP_CONCURRENCY_TEST_HPP