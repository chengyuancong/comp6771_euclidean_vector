#include <catch2/catch.hpp>
#include <comp6771/euclidean_vector.hpp>
#include <sstream>
#include <vector>

/*
   This file is to test all parts integratedly.
   These test assume the the contructors are correctly implemented.
   All test cases calculated and verified by Wolframalpha

Rationale:
   This test would not have any cases that throw exceptions since
   exception test in unit test is sufficient.
*/
TEST_CASE("Overall Test") {
	auto ev1 = comp6771::euclidean_vector{-5.5, 12.14, -11.28, 6.3};
	auto ev2 = comp6771::euclidean_vector{8.29, -1.14, 4.19, -5.28};

	// Test type conversion
	CHECK_THAT(static_cast<std::vector<double>>(ev1),
	           Catch::Approx(std::vector<double>{-5.5, 12.14, -11.28, 6.3}).margin(1e-6));

	CHECK_THAT(static_cast<std::vector<double>>(ev2),
	           Catch::Approx(std::vector<double>{8.29, -1.14, 4.19, -5.28}).margin(1e-6));

	// Test Friend functions
	// + - * /
	CHECK_THAT(static_cast<std::vector<double>>(ev1 * 5.16 + ev2 / (-10.11)),
	           Catch::Approx(std::vector<double>{-29.19998021760633,
	                                             62.75515964391692,
	                                             -58.61924114737883,
	                                             33.03025519287834})
	              .margin(1e-6));

	CHECK_THAT(static_cast<std::vector<double>>(ev1 / 6.4 - ev2 * (-7.31)),
	           Catch::Approx(std::vector<double>{59.74052499999999,
	                                             -6.436525000000001,
	                                             28.866400000000002,
	                                             -37.61242500000001})
	              .margin(1e-6));

	// <<
	auto oss = std::ostringstream{};
	oss << (ev1 / 2.72 - ev2 * 3.14);
	CHECK(oss.str() == "[-28.0527 8.04284 -17.3037 18.8954]");

	// == !=
	CHECK(ev1 != ev2);
	CHECK(ev1 * 0.4 != ev2 * 3.14);
	CHECK(ev1 * 0.8 / 0.8 == ev1);

	// Test member functions
	ev2 += ev1;
	ev2 *= 3.14;

	CHECK_THAT(static_cast<std::vector<double>>(ev2),
	           Catch::Approx(std::vector<double>{8.760599999999998,
	                                             34.54000000000001,
	                                             -22.26260000000001,
	                                             3.202799999999999})
	              .margin(1e-6));

	// Test Utility functions

	CHECK(comp6771::euclidean_norm(ev1) == Approx(18.562273567642514).margin(1e-6));
	CHECK_THAT(static_cast<std::vector<double>>(comp6771::unit(ev1)),
	           Catch::Approx(std::vector<double>{-0.29629991067406314,
	                                             0.6540147119242048,
	                                             -0.6076841804369876,
	                                             0.3393980794993814})
	              .margin(1e-6));

	ev1[0] = 12.01;
	CHECK(comp6771::euclidean_norm(ev1) == Approx(21.41373624569052).margin(1e-6));
	CHECK_THAT(static_cast<std::vector<double>>(comp6771::unit(ev1)),
	           Catch::Approx(std::vector<double>{0.5608549513360609,
	                                             0.5669258209175504,
	                                             -0.5267646836861588,
	                                             0.2942036797183334})
	              .margin(1e-6));

	ev1.at(0) = -5.5;
	CHECK(comp6771::dot(ev1, ev1) == Approx(344.558).margin(1e-6));
}