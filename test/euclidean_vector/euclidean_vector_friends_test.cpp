#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>
#include <sstream>
#include <vector>

/*
This file is to test friend operators works correctly and throw exceptions as expected.
It assumes the Constructors, Vector Type cast overloading and dimension getter is correctly
implemented.

Approach:
    - Construct an euclidean vector
    - Give the euclidean vector to a function call, then
    check if operations and return value are correct
    - check the exception message matched or not  if it should be thrown
*/

/*
Rationale:
   This test ensures it returns true if two given euclidean vectors' value is equal
   and false if not equal.
   Compare an euclidean vector with itself should return true.
   Since two euclidean vectors with different dimensions are definitely not equal,
   it would not throw exception in such case.
*/
TEST_CASE("Equal vec1 == vec2") {
	auto const ev1 = comp6771::euclidean_vector{1.2, -2.3};
	auto const ev2 = comp6771::euclidean_vector{1.2, -2.3};
	auto const ev_diff = comp6771::euclidean_vector{1.2, -2.4};
	auto const ev_zero1 = comp6771::euclidean_vector({});
	auto const ev_zero2 = comp6771::euclidean_vector({});
	SECTION("Compare two distinct euclidean vector") {
		CHECK(ev1 == ev2);
		CHECK(ev_zero1 == ev_zero2);
		CHECK(not(ev1 == ev_diff));
	}

	SECTION("Compare two identical euclidean vector") {
		CHECK(ev1 == ev1);
		CHECK(ev_zero1 == ev_zero1);
	}

	SECTION("Different dimensions") {
		CHECK(not(ev1 == ev_zero1));
	}
}

/*
Rationale:
   This test ensures it returns true if two given euclidean vectors' value is not equal
   and true if not equal.
   Compare an euclidean vector with itself should return false.
   Since two euclidean vectors with different dimensions are definitely not equal,
   it would not throw exception in such case.
*/
TEST_CASE("Not Equal vec1 != vec2") {
	auto const ev1 = comp6771::euclidean_vector{1.2, -2.3};
	auto const ev2 = comp6771::euclidean_vector{1.2, -2.3};
	auto const ev_diff = comp6771::euclidean_vector{1.2, -2.4};
	auto const ev_zero1 = comp6771::euclidean_vector({});
	auto const ev_zero2 = comp6771::euclidean_vector({});
	SECTION("Compare two distinct euclidean vector") {
		CHECK(not(ev1 != ev2));
		CHECK(not(ev_zero1 != ev_zero2));
		CHECK(ev1 != ev_diff);
	}

	SECTION("Compare two identical euclidean vector") {
		CHECK(not(ev1 != ev1));
		CHECK(not(ev_zero1 != ev_zero1));
	}

	SECTION("Different dimensions") {
		CHECK(ev1 != ev_zero1);
	}
}

/*
Rationale:
   Test for addtion ensures it returns a new object
   that adds two euclidean vectors' magnitudes together.

   Test for subtraction ensures it returns a new object
   whose magnitude is left vector's magnitude subtract right  vector's magnitude.

   Both tests ensure 'dimension unequal' exceptions are thrown with correct message
*/
TEST_CASE("Addtion and Subtraction with two euclidean vectors") {
	SECTION("Addition vec1 + vec2") {
		auto const ev1 = comp6771::euclidean_vector{1.2, -2.3};
		auto const ev2 = comp6771::euclidean_vector{-3.4, 4.5};
		auto const ev_zero = comp6771::euclidean_vector({});
		SECTION("Add two non-zero dimension euclidean vector") {
			auto const new_ev = ev1 + ev2;
			auto const exp = std::vector<double>{-2.2, 2.2};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Add two zero dimension euclidean vector") {
			auto const new_ev = ev_zero + ev_zero;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Adding two different dimensions euclidean vector") {
			CHECK_THROWS_MATCHES(ev1 + ev_zero,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Dimensions of LHS(2) and RHS(0) do not "
			                                              "match"));
		}
	}

	SECTION("Subtraction vec1 - vec2") {
		auto const ev1 = comp6771::euclidean_vector{1.2, -2.3};
		auto const ev2 = comp6771::euclidean_vector{-3.4, 4.5};
		auto const ev_zero1 = comp6771::euclidean_vector({});
		auto const ev_zero2 = comp6771::euclidean_vector({});
		SECTION("Subtraction with two non-zero dimension euclidean vector") {
			auto const new_ev = ev1 - ev2;
			auto const exp = std::vector<double>{4.6, -6.8};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Subtraction with two zero dimension euclidean vector") {
			auto const new_ev = ev_zero1 - ev_zero2;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Adding two different dimensions euclidean vector") {
			CHECK_THROWS_MATCHES(ev1 - ev_zero1,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Dimensions of LHS(2) and RHS(0) do not "
			                                              "match"));
		}
	}
}

/*
Rationale:
   - This test ensures an euclidean vector's magnitude is
   correctly multiplied or divided by a given scalar.
   - Since scalar can be on both LHS and RHS of operator*,
   this test should ensures both cases works
   - This test ensures divided by 0 exceptions are thrown with correct message
*/
TEST_CASE("Multiplication and Division with an euclidean vector and a scalar") {
	SECTION("Multiplication vec * scalar") {
		auto ev = comp6771::euclidean_vector{1.2, -2.4};
		auto ev_zero = comp6771::euclidean_vector({});
		SECTION("An euclidean vector multiplies a non-zero scalar") {
			auto const new_ev = ev * 2;
			auto const exp = std::vector<double>{2.4, -4.8};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("An euclidean vector multiplies 0") {
			auto const new_ev = ev * 0;
			auto const exp = std::vector<double>{0, 0};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("A zero dimension euclidean vector multiplies a scalar") {
			auto const new_ev1 = ev_zero * 2;
			auto const new_ev2 = ev_zero * 0;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev1), Catch::Approx(exp).margin(1e-6));
			CHECK_THAT(static_cast<std::vector<double>>(new_ev2), Catch::Approx(exp).margin(1e-6));
		}
	}

	SECTION("Multiplication scalar * vec") {
		auto ev = comp6771::euclidean_vector{1.2, -2.4};
		auto ev_zero = comp6771::euclidean_vector({});
		SECTION("A non-zero scalar multiplies an euclidean vector") {
			auto const new_ev = 2 * ev;
			auto const exp = std::vector<double>{2.4, -4.8};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("0 multiplies an euclidean vector") {
			auto const new_ev = 0 * ev;
			auto const exp = std::vector<double>{0, 0};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("A scalar multiplies a zero dimension euclidean vector ") {
			auto const new_ev1 = 2 * ev_zero;
			auto const new_ev2 = 0 * ev_zero;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev1), Catch::Approx(exp).margin(1e-6));
			CHECK_THAT(static_cast<std::vector<double>>(new_ev2), Catch::Approx(exp).margin(1e-6));
		}
	}

	SECTION("Division vec / scalar") {
		auto ev = comp6771::euclidean_vector{1.2, -2.4};
		auto ev_zero = comp6771::euclidean_vector({});
		SECTION("An euclidean vector divided by a non-zero scalar") {
			auto const new_ev = ev / 2;
			auto const exp = std::vector<double>{0.6, -1.2};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("An euclidean vector divided by 0") {
			CHECK_THROWS_MATCHES(ev / 0,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Invalid vector division by 0"));
			CHECK_THROWS_MATCHES(ev_zero / 0,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Invalid vector division by 0"));
		}

		SECTION("Zero dimension euclidean vector divided by a non-zero scalar") {
			auto const new_ev = ev_zero / 2;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
		}
	}
}

/*
Rationale:
    This test ensures the output of << is in correct value and format.
*/
TEST_CASE("Output Stream << ") {
	SECTION("Empty euclidean vector") {
		auto const empty = comp6771::euclidean_vector(0);
		auto oss = std::ostringstream{};
		oss << empty;
		CHECK(oss.str() == "[]");
	}

	SECTION("Non-empty euclidean vector") {
		auto const non_empty = comp6771::euclidean_vector();
		auto oss = std::ostringstream{};
		oss << non_empty;
		CHECK(oss.str() == "[0]");
	}

	SECTION("Int magnitude") {
		auto const int_ev = comp6771::euclidean_vector{1, 2, 3, 4, 5};
		auto oss = std::ostringstream{};
		oss << int_ev;
		CHECK(oss.str() == "[1 2 3 4 5]");
	}

	SECTION("Double magnitude") {
		auto const double_ev =
		   comp6771::euclidean_vector{1.1, 2.000001, 3.1234567, 4.123451, 5.111, 6.10000};
		auto oss = std::ostringstream{};
		oss << double_ev;
		CHECK(oss.str() == "[1.1 2 3.12346 4.12345 5.111 6.1]");
	}

	SECTION("Mixed magnitude") {
		auto const mixed_ev = comp6771::euclidean_vector{1, 2.000001, 3, 4.123455, 5, 6.000006};
		auto oss = std::ostringstream{};
		oss << mixed_ev;
		CHECK(oss.str() == "[1 2 3 4.12345 5 6.00001]");
	}

	SECTION("Continuously print multiple euclidean vectors") {
		auto const int_ev = comp6771::euclidean_vector{1, 2, 3, 4, 5};
		auto const double_ev =
		   comp6771::euclidean_vector{1.1, 2.000001, 3.1234567, 4.123451, 5.111, 6.10000};
		auto oss = std::ostringstream{};
		oss << int_ev << double_ev;=
		CHECK(oss.str() == "[1 2 3 4 5][1.1 2 3.12346 4.12345 5.111 6.1]");
	}
}