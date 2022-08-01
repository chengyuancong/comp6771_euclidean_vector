#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>
#include <vector>

/*
This file is to test all constructors.
It assumes the Vector Type cast overloading and dimension getter are correctly implemented.
Approach:
    - Construct an euclidean vector
    - Construct an expected std::vector that contains same elements
    - cast euclidean vector to std::vector and compare it with expected result
*/

/*
Rationale:
    For normal constructors, comparing magnitudes with expected vector ensures that both values and
   dimensions are correctly constructed in euclidean vector.
*/
TEST_CASE("Default Constructor") {
	auto const ev = comp6771::euclidean_vector();
	auto const vec_exp = std::vector<double>{0};
	CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
}

TEST_CASE("Single-argument Constructor") {
	SECTION("Zero dimension") {
		auto const ev = comp6771::euclidean_vector(0);
		auto const vec_exp = std::vector<double>{};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}

	SECTION("Non-zero dimension") {
		auto const ev = comp6771::euclidean_vector(2);
		auto const vec_exp = std::vector<double>{0, 0};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}
}

TEST_CASE("Constructor: dimensions and magnitude") {
	SECTION("Zero dimension") {
		auto const ev = comp6771::euclidean_vector(0, 0);
		auto const vec_exp = std::vector<double>{};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}

	SECTION("Non-zero dimension") {
		auto const ev = comp6771::euclidean_vector(1, 5.5);
		auto const vec_exp = std::vector<double>{5.5};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}
}

TEST_CASE("Constructor: start and end iterator of an vector") {
	SECTION("Empty vector") {
		auto const input_vec = std::vector<double>{};
		auto const ev = comp6771::euclidean_vector(input_vec.cbegin(), input_vec.cend());
		auto const vec_exp = std::vector<double>{};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}

	SECTION("Non-empty vector") {
		auto const input_vec = std::vector<double>{1.1, -2.2, 3.3, -4.4, 5.5};
		auto const ev = comp6771::euclidean_vector(input_vec.cbegin(), input_vec.cend());
		auto const vec_exp = std::vector<double>{1.1, -2.2, 3.3, -4.4, 5.5};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}
}

TEST_CASE("Constructor: an initialiser list") {
	SECTION("Empty initialiser list without parentheses") {
		// This should call default constructor
		auto const ev = comp6771::euclidean_vector{};
		auto const vec_exp = std::vector<double>{0};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}

	SECTION("Empty initialiser list in parentheses") {
		// This should call constructor that takes an empty initialiser list
		auto const ev = comp6771::euclidean_vector({});
		auto const vec_exp = std::vector<double>{};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}

	SECTION("Non-empty initialiser list") {
		auto const ev = comp6771::euclidean_vector{1.1, -2.2, 3.3, -4.4, 5.5};
		auto const vec_exp = std::vector<double>{1.1, -2.2, 3.3, -4.4, 5.5};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}
}

/*
Rationale:
    For copy constructor, this test ensures that original object and the
    copy constructed object are distinct and their magnitudes are equal.
*/
TEST_CASE("Copy Constructor") {
	auto const ev = comp6771::euclidean_vector{1.2, -2.3};
	auto ev_copy = ev;
	auto const vec_exp = std::vector<double>{1.2, -2.3};

	SECTION("Original object and copy has same magnitude") {
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
		CHECK_THAT(static_cast<std::vector<double>>(ev_copy), Catch::Approx(vec_exp).margin(1e-6));
	}

	// This section assumes operator[] is correctly implemented
	SECTION("Modifying copy does not affect original object") {
		ev_copy[0] = 0;
		auto const copy_exp = std::vector<double>{0, -2.3};

		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
		CHECK_THAT(static_cast<std::vector<double>>(ev_copy), Catch::Approx(copy_exp).margin(1e-6));
	}
}

/*
Rationale:
    For move constructor, this test ensures that magnitude of
    original object(before moved) equals move constructed object
    and original object's dimension has been set to 0.
*/
TEST_CASE("Move Constructor") {
	auto ev = comp6771::euclidean_vector{1.2, -2.3};
	auto const vec_exp = std::vector<double>{1.2, -2.3};
	auto const move = std::move(ev);

	SECTION("Check magnitude and dimensions of move-to object") {
		CHECK_THAT(static_cast<std::vector<double>>(move), Catch::Approx(vec_exp).margin(1e-6));
		CHECK(move.dimensions() == 2);
	}

	SECTION("Check dimensions of original object") {
		// NOLINTNEXTLINE(bugprone-use-after-move)
		CHECK(ev.dimensions() == 0);
	}
}
