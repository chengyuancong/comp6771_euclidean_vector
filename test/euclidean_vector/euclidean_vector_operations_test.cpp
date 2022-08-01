#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>
#include <vector>

/*
This file is to test whether non-friend operators works correctly and throw exceptions as expected.
It assumes the Constructors, Vector Type cast overloading and dimension getter is correctly
implemented.

Approach:
    - Construct an euclidean vector
    - Use these operators on 'this' object
    - Check the return value if the operator is marked 'const',
        or check whether 'this' object is correctly modified
    - Check the exception message matched or not if it should be thrown
*/

/*
Rationale:
    For copy assignment, this test ensures that original object
    and the copy assigned object are distinct and their magnitudes are equal.
*/
TEST_CASE("Copy Assignment") {
	auto const origin = comp6771::euclidean_vector{1.2, -2.3};
	auto copy = comp6771::euclidean_vector({});

	auto const pre_copy_exp = std::vector<double>{};
	CHECK_THAT(static_cast<std::vector<double>>(copy), Catch::Approx(pre_copy_exp).margin(1e-6));

	// copy assignment
	copy = origin;

	SECTION("Original object and copy has same value") {
		auto const post_copy_exp = std::vector<double>{1.2, -2.3};
		CHECK_THAT(static_cast<std::vector<double>>(copy), Catch::Approx(post_copy_exp).margin(1e-6));
	}

	// This section assumes operator[] is correctly implemented
	SECTION("Modifying copy does not affect original object") {
		copy[0] = 0;
		auto const origin_exp = std::vector<double>{1.2, -2.3};
		auto const copy_exp = std::vector<double>{0, -2.3};

		CHECK_THAT(static_cast<std::vector<double>>(origin), Catch::Approx(origin_exp).margin(1e-6));
		CHECK_THAT(static_cast<std::vector<double>>(copy), Catch::Approx(copy_exp).margin(1e-6));
	}
}

/*
Rationale:
    For move assignment, this test ensures that magnitude of
    original object(before moved) equals the move assigned object
    and original object's dimension has been set to 0.

*/
TEST_CASE("Move Assignment") {
	auto origin = comp6771::euclidean_vector{1.2, -2.3};
	auto move = comp6771::euclidean_vector({});

	auto const pre_move_exp = std::vector<double>{};
	CHECK_THAT(static_cast<std::vector<double>>(move), Catch::Approx(pre_move_exp).margin(1e-6));

	// move assignment
	move = std::move(origin);

	SECTION("Check magnitude and dimensions of move-to object") {
		auto const post_move_exp = std::vector<double>{1.2, -2.3};
		CHECK_THAT(static_cast<std::vector<double>>(move), Catch::Approx(post_move_exp).margin(1e-6));
		CHECK(move.dimensions() == 2);
	}

	SECTION("Check dimensions of original object") {
		// NOLINTNEXTLINE(bugprone-use-after-move)
		CHECK(origin.dimensions() == 0);
	}
}

/*
Rationale:
   Constant subscript returns a double value at given index.
   Non-constant subscript returns a double reference at given index.

   This test ensures that const subscript returns a correct value and non-const subscript
   correctly modifies value of given index.

   Since subscript would not check index range and  throw exceptions,
   this test only checks cases with valid inputes.
*/
TEST_CASE("Subscript") {
	SECTION("Const Subscript") {
		auto ev = comp6771::euclidean_vector{1.2, -2.3};
		auto const const_ev = comp6771::euclidean_vector{1.2, -2.3};
		double a{ev[1]};
		double b{const_ev[1]};
		CHECK(a == Approx(-2.3).margin(1e-6));
		CHECK(b == Approx(-2.3).margin(1e-6));
	}

	SECTION("Non-const Subscript") {
		auto ev = comp6771::euclidean_vector{1.2, -2.3};
		ev[1] = 3.1;
		auto const vec_exp = std::vector<double>{1.2, 3.1};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(vec_exp).margin(1e-6));
	}
}

/*
Rationale:
   Test for Unary plus ensures it returns a copy of *this.
   Test for negation ensures it returns a copy of *this but with all magnitude negated.
   Both tests ensures 'this' object is not modified after using these const operators.

   This test assumes copy constructor is correctly implemented. Since they returned by value,
   new euclidean vector will definitely be distinct from the original one.
*/
TEST_CASE("Unary plus and minus") {
	auto ev = comp6771::euclidean_vector{1.2, -2.3};
	auto const exp = std::vector<double>{1.2, -2.3};

	SECTION("Unary plus") {
		auto const new_ev = +ev;
		ev = +ev;
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(exp).margin(1e-6));
		CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(exp).margin(1e-6));
	}

	SECTION("Negation") {
		auto const new_ev = -ev;
		auto const new_exp = std::vector<double>{-1.2, 2.3};
		CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(exp).margin(1e-6));
		CHECK_THAT(static_cast<std::vector<double>>(new_ev), Catch::Approx(new_exp).margin(1e-6));
	}
}

/*
Rationale:
   - This test ensures another euclidean vector's magnitude is
   correctly added or subtracted to 'this' euclidean vector.
   - This test ensures the modification is correct when operate on themselves
   - This test ensures 'dimension unequal' exceptions are thrown with correct message
*/
TEST_CASE("Compound Addition and Subtraction between euclidean vectors") {
	auto ev1 = comp6771::euclidean_vector{1.2, -2.3};
	auto ev2 = comp6771::euclidean_vector{-3.4, 4.5};
	auto ev3_zero = comp6771::euclidean_vector({});
	auto ev4_zero = comp6771::euclidean_vector({});
	SECTION("Compound Addition") {
		SECTION("Correctly added with another euclidean vector") {
			ev1 += ev2;
			auto const exp = std::vector<double>{-2.2, 2.2};
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly added by itself") {
			ev1 += ev1;
			auto const exp = std::vector<double>{2.4, -4.6};
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly operate zero dimension euclidean vector") {
			ev3_zero += ev4_zero;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(ev3_zero), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly throw dimension unequal exception") {
			CHECK_THROWS_MATCHES(ev1 += ev3_zero,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Dimensions of LHS(2) and RHS(0) do not "
			                                              "match"));
		}
	}

	SECTION("Compound Subtraction") {
		SECTION("Correctly subtracted with another euclidean vector") {
			ev1 -= ev2;
			auto const exp = std::vector<double>{4.6, -6.8};
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly subtracted by itself") {
			ev1 -= ev1;
			auto const exp = std::vector<double>{0, 0};
			CHECK_THAT(static_cast<std::vector<double>>(ev1), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly operated zero dimension euclidean vector") {
			ev3_zero -= ev4_zero;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(ev3_zero), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly throw dimension unequal exception") {
			CHECK_THROWS_MATCHES(ev1 -= ev3_zero,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Dimensions of LHS(2) and RHS(0) do not "
			                                              "match"));
		}
	}
}

/*
Rationale:
   - This test ensures another euclidean vector's magnitude is
   correctly multiplied or divided by a given scalar.
   - This test ensures divided by 0 exceptions are thrown with correct message
*/
TEST_CASE("Compound Multiplication and Division between an euclidean vector and a scalar") {
	auto ev = comp6771::euclidean_vector{1.2, -2.4};
	auto ev_zero = comp6771::euclidean_vector({});
	SECTION("Compound Multiplication") {
		SECTION("Correctly multiplied by a non-zero scalar") {
			ev *= 2;
			auto const exp = std::vector<double>{2.4, -4.8};
			CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly multiplied by 0") {
			ev *= 0;
			auto const exp = std::vector<double>{0, 0};
			CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly operated zero dimension euclidean vector") {
			ev_zero *= 3;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(ev_zero), Catch::Approx(exp).margin(1e-6));
			ev_zero *= 0;
			CHECK_THAT(static_cast<std::vector<double>>(ev_zero), Catch::Approx(exp).margin(1e-6));
		}
	}

	SECTION("Compound Division") {
		SECTION("Correctly divided by a non-zero scalar") {
			ev /= 2;
			auto const exp = std::vector<double>{0.6, -1.2};
			CHECK_THAT(static_cast<std::vector<double>>(ev), Catch::Approx(exp).margin(1e-6));
		}

		SECTION("Correctly throw divided by 0 exception") {
			CHECK_THROWS_MATCHES(ev /= 0,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Invalid vector division by 0"));
			CHECK_THROWS_MATCHES(ev_zero /= 0,
			                     comp6771::euclidean_vector_error,
			                     Catch::Matchers::Message("Invalid vector division by 0"));
		}

		SECTION("Correctly operated zero dimension euclidean vector") {
			ev_zero /= 3;
			auto const exp = std::vector<double>{};
			CHECK_THAT(static_cast<std::vector<double>>(ev_zero), Catch::Approx(exp).margin(1e-6));
		}
	}
}

/*
Rationale:
   This test ensures returned std::vector equals expected result.
*/
TEST_CASE("Vector Type Conversion") {
	auto const ev = comp6771::euclidean_vector{1.2, -2.3, 3.4};
	auto const ev_zero = comp6771::euclidean_vector({});
	SECTION("Correctly cast 0 dimensions euclidean vectors") {
		auto const exp = std::vector<double>{};
		CHECK(static_cast<std::vector<double>>(ev_zero) == exp);
	}

	SECTION("Correctly cast non-zero dimensions euclidean vectors") {
		auto const exp = std::vector<double>{1.2, -2.3, 3.4};
		CHECK(static_cast<std::vector<double>>(ev) == exp);
	}
}

/*
Rationale:
   This test ensures returned std::list equals expected result.
*/
TEST_CASE("List Type Conversion") {
	auto const ev = comp6771::euclidean_vector{1.2, -2.3, 3.4};
	auto const ev_zero = comp6771::euclidean_vector({});
	SECTION("Correctly cast 0 dimensions euclidean vectors") {
		auto const exp = std::list<double>{};
		CHECK(static_cast<std::list<double>>(ev_zero) == exp);
	}

	SECTION("Correctly cast non-zero dimensions euclidean vectors") {
		auto const exp = std::list<double>{1.2, -2.3, 3.4};
		CHECK(static_cast<std::list<double>>(ev) == exp);
	}
}