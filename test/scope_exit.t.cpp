#include <scope_exit/scope_exit.hpp>

#include <catch2/catch_test_macros.hpp>

#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

TEST_CASE("basic scope_exit functionality", "[scope_exit][basic]") {
    SECTION("simple scope exit execution") {
        bool executed = false;

        {
            scope(exit) {
                executed = true;
            };
        }

        REQUIRE(executed == true);
    }

    SECTION("multiple scope exits execute in LIFO order") {
        std::vector<int> order;

        {
            scope(exit) {
                order.push_back(1);
            };

            scope(exit) {
                order.push_back(2);
            };

            scope(exit) {
                order.push_back(3);
            };
        }

        REQUIRE(order == std::vector<int>{3, 2, 1});
    }

    SECTION("nested scopes with scope exits") {
        std::vector<std::string> order;

        {
            scope(exit) {
                order.push_back("outer");
            };

            {
                scope(exit) {
                    order.push_back("inner");
                };
            }
        }

        REQUIRE(order == std::vector<std::string>{"inner", "outer"});
    }
}

TEST_CASE("variable capture and modification", "[scope_exit][capture]") {
    SECTION("capture by reference and modify") {
        int counter = 0;

        {
            scope(exit) {
                ++counter;
            };

            REQUIRE(counter == 0); // not executed yet
        }

        REQUIRE(counter == 1); // executed on scope exit
    }

    SECTION("access local variables") {
        int multiplier = 5;
        int result = 0;

        {
            int value = 10;
            scope(exit) {
                result = value * multiplier;
            };
        }

        REQUIRE(result == 50);
    }

    SECTION("mutable lambda state") {
        int final_count = 0;

        {
            scope(exit) {
                static int call_count = 0;
                ++call_count;
                final_count = call_count;
            };
        }

        REQUIRE(final_count == 1);
    }

    SECTION("multiple variables captured") {
        std::string result;
        int num = 42;
        std::string text = "Answer: ";

        {
            scope(exit) {
                result = text + std::to_string(num);
            };
        }

        REQUIRE(result == "Answer: 42");
    }
}

TEST_CASE("exception handling", "[scope_exit][exceptions]") {
    SECTION("scope exit executes during exception unwinding") {
        bool cleanup_executed = false;

        try {
            scope(exit) {
                cleanup_executed = true;
            };

            throw std::runtime_error("test exception");
        }
        catch (std::exception &) {
            // exception caught
        }

        REQUIRE(cleanup_executed == true);
    }

    SECTION("multiple scope exits with exception") {
        std::vector<int> cleanup_order;

        try {
            scope(exit) {
                cleanup_order.push_back(1);
            };

            scope(exit) {
                cleanup_order.push_back(2);
            };

            throw std::runtime_error("test exception");
        }
        catch (std::exception &) {
            // exception caught
        }

        REQUIRE(cleanup_order == std::vector<int>{2, 1});
    }
}

TEST_CASE("edge cases and control flow", "[scope_exit][edge_cases]") {

    SECTION("empty scope exit block") {
        bool reached_after = false;

        {
            scope(exit) {
                // empty block - should compile and run without issues
            };

            reached_after = true;
        }

        REQUIRE(reached_after == true);
    }

    SECTION("scope exit in loop") {
        std::vector<int> iterations;

        for (int i = 0; i < 3; ++i) {
            scope(exit) {
                iterations.push_back(i);
            };
        }

        REQUIRE(iterations == std::vector<int>{0, 1, 2});
    }

    SECTION("scope exit with early return") {
        bool cleanup_executed = false;

        auto test_function = [&]() -> bool {
            scope(exit) {
                cleanup_executed = true;
            };

            if (true) {
                return false; // Early return
            }

            return true;
        };

        bool result = test_function();

        REQUIRE(result == false);
        REQUIRE(cleanup_executed == true);
    }

    SECTION("scope exit in conditional blocks") {
        std::vector<std::string> order;
        bool condition = true;

        {
            if (condition) {
                scope(exit) {
                    order.push_back("if_block");
                };
            } else {
                scope(exit) {
                    order.push_back("else_block");
                };
            }

            scope(exit) {
                order.push_back("outer");
            };
        }

        REQUIRE(order == std::vector<std::string>{"if_block", "outer"});
    }
}
