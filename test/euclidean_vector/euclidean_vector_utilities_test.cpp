#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>
#include <vector>

/*
This file is to test utility functions.
It assumes the Constructors, Vector Type cast overloading and dimension getter is correctly
implemented.

Approach:
    - Construct an euclidean vector
    - Call utility functions on it, then check if the return value equals expected result.
    - Check the exception message matched or not if it should be thrown
*/

/*
Rationale:
    This test ensures a correct norm of an euclidean vector is returned.
*/
TEST_CASE("Euclidean norm") {
	SECTION("Norm of a non-zero dimension euclidean vector") {
		auto const ev1 = comp6771::euclidean_vector{3, 4, 5};
		auto const ev2 = comp6771::euclidean_vector{1, 2, 2};
		auto const ev3 = comp6771::euclidean_vector{1.2, -2.3, 3.4};

		CHECK(comp6771::euclidean_norm(ev1) == Approx(7.0710678).margin(1e-6));
		CHECK(comp6771::euclidean_norm(ev2) == Approx(3).margin(1e-6));
		CHECK(comp6771::euclidean_norm(ev3) == Approx(4.2766809).margin(1e-6));
	}

	SECTION("Norm of a zero dimension euclidean vector") {
		auto const ev = comp6771::euclidean_vector({});
		CHECK(comp6771::euclidean_norm(ev) == 0);
	}
}

/*
Rationale:
    This test ensures a correct unit vector of an euclidean vector is returned.
    It should also check if an exception is throw when
    this euclidean vector doesn't have a unit vector.
*/
TEST_CASE("Unit vector") {
	SECTION("Unit vector of a not-zero dimension and non-zero norm euclidean vector") {
		auto const ev = comp6771::euclidean_vector{1, 2, 2};
		auto const unit = comp6771::unit(ev);
		auto const unit_exp = std::vector<double>{0.3333333, 0.6666667, 0.6666667};
		CHECK_THAT(static_cast<std::vector<double>>(comp6771::unit(ev)),
		           Catch::Approx(unit_exp).margin(1e-6));
	}

	SECTION("Unit vector of a zero dimension euclidean vector") {
		auto const ev_zero_dim = comp6771::euclidean_vector({});
		CHECK_THROWS_MATCHES(comp6771::unit(ev_zero_dim),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("euclidean_vector with no dimensions does not "
		                                              "have a unit vector"));
	}

	SECTION("Unit vector of a zero norm euclidean vector") {
		auto const ev_zero_norm = comp6771::euclidean_vector{0, 0};
		CHECK_THROWS_MATCHES(comp6771::unit(ev_zero_norm),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("euclidean_vector with zero euclidean normal "
		                                              "does not have a unit vector"));
	}
}

/*
Rationale:
    This test ensures a correct dot product of two equal-dimensions euclidean vector is returned.
    It should also check if an exception is throw when
    these two euclidean vectors' dimension are not equal.
*/
TEST_CASE("Dot product") {
	auto const ev1 = comp6771::euclidean_vector{1, 2};
	auto const ev2 = comp6771::euclidean_vector{3, 4};
	auto const ev3 = comp6771::euclidean_vector{-1.2, 2.3};
	auto const ev_zero_dim = comp6771::euclidean_vector({});

	SECTION("Dot product of two equal-dimensions euclidean vector") {
		SECTION("Dot product of two zero dimensions euclidean vector") {
			CHECK(comp6771::dot(ev_zero_dim, ev_zero_dim) == Approx(0).margin(1e-6));
		}

		SECTION("Dot product of two non-zero dimensions euclidean vector") {
			CHECK(comp6771::dot(ev1, ev2) == Approx(11).margin(1e-6));
			CHECK(comp6771::dot(ev1, ev3) == Approx(3.4).margin(1e-6));
		}
	}

	SECTION("Dot product of two euclidean vectors when dimensions are not equal") {
		CHECK_THROWS_MATCHES(comp6771::dot(ev1, ev_zero_dim),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Dimensions of LHS(2) and RHS(0) do not "
		                                              "match"));
	}
}
