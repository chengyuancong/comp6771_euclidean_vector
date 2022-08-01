#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what)
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		// Constructors
		euclidean_vector();

		explicit euclidean_vector(int dimensions);

		euclidean_vector(int dimensions, double magnitude);

		euclidean_vector(std::vector<double>::const_iterator cbegin,
		                 std::vector<double>::const_iterator cend);

		euclidean_vector(std::initializer_list<double> list);

		// Rule of 5!
		// Copy constructor
		euclidean_vector(euclidean_vector const& orig);

		// Move constructor
		euclidean_vector(euclidean_vector&& orig) noexcept;

		// Destructor
		~euclidean_vector() = default;

		// Copy assignment
		auto operator=(euclidean_vector const& orig) -> euclidean_vector&;

		// Move assignment
		auto operator=(euclidean_vector&& orig) noexcept -> euclidean_vector&;

		// Operations
		auto operator[](int index) const noexcept -> double;
		auto operator[](int index) noexcept -> double&;

		auto operator+() const noexcept -> euclidean_vector;
		auto operator-() const noexcept -> euclidean_vector;

		auto operator+=(euclidean_vector const& other) -> euclidean_vector&;
		auto operator-=(euclidean_vector const& other) -> euclidean_vector&;

		auto operator*=(double scalar) noexcept -> euclidean_vector&;
		auto operator/=(double scalar) -> euclidean_vector&;

		explicit operator std::vector<double>() const noexcept;
		explicit operator std::list<double>() const noexcept;

		// Member functions
		[[nodiscard]] auto at(int index) const -> double;
		auto at(int index) -> double&;
		[[nodiscard]] auto dimensions() const noexcept -> int;

		// Friends
		friend auto operator==(euclidean_vector const& vec1, euclidean_vector const& vec2) noexcept
		   -> bool {
			// equal if they are same object, otherwise compare dimensions and magnitude
			return std::addressof(vec1) == std::addressof(vec2)
			       or (vec1.dimensions_ == vec2.dimensions_
			           and std::equal(vec1.magnitude_.get(),
			                          vec1.magnitude_.get() + vec1.dimensions_,
			                          vec2.magnitude_.get(),
			                          vec2.magnitude_.get() + vec2.dimensions_,
			                          [](double const& val1, double const& val2) {
				                          return std::abs(val1 - val2) < 1e-6;
			                          }));
		};

		friend auto operator!=(euclidean_vector const& vec1, euclidean_vector const& vec2) noexcept
		   -> bool {
			return not(vec1 == vec2);
		};

		friend auto operator+(euclidean_vector const& vec1, euclidean_vector const& vec2)
		   -> euclidean_vector {
			auto new_vec = vec1;
			new_vec += vec2;
			return new_vec;
		};

		friend auto operator-(euclidean_vector const& vec1, euclidean_vector const& vec2)
		   -> euclidean_vector {
			auto new_vec = vec1;
			new_vec -= vec2;
			return new_vec;
		};

		friend auto operator*(euclidean_vector const& vec, double scalar) noexcept -> euclidean_vector {
			auto new_vec = vec;
			new_vec *= scalar;
			return new_vec;
		};

		friend auto operator*(double scalar, euclidean_vector const& vec) noexcept -> euclidean_vector {
			return vec * scalar;
		};

		friend auto operator/(euclidean_vector const& vec, double scalar) -> euclidean_vector {
			auto new_vec = vec;
			new_vec /= scalar;
			return new_vec;
		};

		friend auto operator<<(std::ostream& os, euclidean_vector const& vec) noexcept
		   -> std::ostream& {
			auto oss = std::ostringstream();
			oss.precision(6);
			std::for_each (vec.magnitude_.get(),
			               vec.magnitude_.get() + vec.dimensions_,
			               [&oss](double const& val) { oss << val << " "; });
			return (os << "[" << oss.str().substr(0, oss.str().size() - 1) << "]");
		};

		// Wrapper of std::inner_product()
		friend auto euclidean_inner_product(euclidean_vector const& vec1,
		                                    euclidean_vector const& vec2) noexcept -> double {
			return std::inner_product(vec1.magnitude_.get(),
			                          vec1.magnitude_.get() + vec1.dimensions_,
			                          vec2.magnitude_.get(),
			                          0.0);
		};

	private:
		int dimensions_;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitude_;
	};

	// Utility functions
	auto euclidean_norm(euclidean_vector const& v) noexcept -> double;
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

	// Helper functions
	auto check_dimensions_equal(euclidean_vector const& vec1, euclidean_vector const& vec2) -> void;
	auto check_index_valid(euclidean_vector const& vec, int index) -> void;
} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
