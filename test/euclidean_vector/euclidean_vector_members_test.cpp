#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>
#include <vector>

/*
This file is to test all member functions.
It assumes that the constructors and casting to std::vector are correct.
Approach:
    - Call member functions on 'this' object, check if return value is correct.
    - Call non const functions to modify object, check if 'this' is correctly modified.
*/

/*
Rationale:
   - This test ensures function 'const At' returns correct value
   on both const and non-const euclidean vector.
   - This test ensures 'invalid index' exceptions are thrown with correct message
*/
TEST_CASE("Const At") {
	auto ev = comp6771::euclidean_vector{1.2, -2.3};
	auto const const_ev = comp6771::euclidean_vector{1.2, -2.3};
	SECTION("Returns correct value with valid index") {
		double a{ev.at(1)};
		double b{const_ev.at(1)};
		CHECK(a == Approx(-2.3).margin(1e-6));
		CHECK(b == Approx(-2.3).margin(1e-6));
	}

	SECTION("Throws exceptions with invalid index") {
		double a{0};
		CHECK_THROWS_MATCHES(a = ev.at(2),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Index 2 is not valid for this "
		                                              "euclidean_vector object"));
		CHECK_THROWS_MATCHES(a = const_ev.at(2),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Index 2 is not valid for this "
		                                              "euclidean_vector object"));
		CHECK_THROWS_MATCHES(a = ev.at(-1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Index -1 is not valid for this "
		                                              "euclidean_vector object"));
		CHECK_THROWS_MATCHES(a = const_ev.at(-1),
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Index -1 is not valid for this "
		                                              "euclidean_vector object"));
	}
}

/*
Rationale:
   - This test ensures function 'non-const At' returns correct reference
   and can be used to modify magnitude of non-const euclidean vector.
   - This test ensures 'invalid index' exceptions are thrown with correct message
*/
TEST_CASE("Non-const At") {
	auto ev = comp6771::euclidean_vector{1.2, -2.3};
	SECTION("Returns correct reference to magnitude and can be used to modify it") {
		ev.at(1) = 3.1;
		auto const exp = std::vector<double>{1.2, 3.1};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(exp).margin(1e-6));
	}

	SECTION("Throws exceptions with invalid index") {
		CHECK_THROWS_MATCHES(ev.at(2) = 3.1,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Index 2 is not valid for this "
		                                              "euclidean_vector object"));
		CHECK_THROWS_MATCHES(ev.at(-1) = 3.1,
		                     comp6771::euclidean_vector_error,
		                     Catch::Matchers::Message("Index -1 is not valid for this "
		                                              "euclidean_vector object"));
	}
}

/*
Rationale:
   - This test ensures function 'dimensions' returns correct dimension
*/
TEST_CASE("Dimensions") {
	auto ev = comp6771::euclidean_vector{1.2, -2.3};
	auto const const_ev = comp6771::euclidean_vector{1.2, -2.3};
	CHECK(ev.dimensions() == 2);
	CHECK(const_ev.dimensions() == 2);
}
