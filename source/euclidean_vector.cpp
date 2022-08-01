// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include <comp6771/euclidean_vector.hpp>

namespace comp6771 {
	// Constructors
	euclidean_vector::euclidean_vector()
	: euclidean_vector(1, 0){};

	euclidean_vector::euclidean_vector(int dimensions)
	: euclidean_vector(dimensions, 0){};

	euclidean_vector::euclidean_vector(int dimensions, double magnitude)
	: dimensions_{dimensions} // NOLINTNEXTLINE(modernize-avoid-c-arrays)
	, magnitude_{std::make_unique<double[]>(static_cast<size_t>(dimensions_))} {
		std::fill(magnitude_.get(), magnitude_.get() + dimensions, magnitude);
	};

	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator cbegin,
	                                   std::vector<double>::const_iterator cend)
	: dimensions_{static_cast<int>(std::distance(cbegin, cend))}
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	, magnitude_{std::make_unique<double[]>(static_cast<size_t>(dimensions_))} {
		std::copy(cbegin, cend, magnitude_.get());
	};

	euclidean_vector::euclidean_vector(std::initializer_list<double> list)
	: dimensions_{static_cast<int>(list.size())} // NOLINTNEXTLINE(modernize-avoid-c-arrays)
	, magnitude_{std::make_unique<double[]>(static_cast<size_t>(dimensions_))} {
		std::copy(list.begin(), list.end(), magnitude_.get());
	};

	// Copy constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& orig)
	: dimensions_{orig.dimensions_} // NOLINTNEXTLINE(modernize-avoid-c-arrays)
	, magnitude_{std::make_unique<double[]>(static_cast<size_t>(dimensions_))} {
		std::copy(orig.magnitude_.get(), orig.magnitude_.get() + dimensions_, magnitude_.get());
	};

	// Move constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& orig) noexcept
	: dimensions_{std::exchange(orig.dimensions_, 0)}
	, magnitude_{std::move(orig.magnitude_)} {};

	// Copy assignment
	auto euclidean_vector::operator=(euclidean_vector const& orig) -> euclidean_vector& {
		auto copy = orig;
		std::swap(copy, *this);
		return *this;
	};

	// Move assignment
	auto euclidean_vector::operator=(euclidean_vector&& orig) noexcept -> euclidean_vector& {
		std::swap(dimensions_, orig.dimensions_);
		orig.dimensions_ = 0;
		magnitude_ = std::move(orig.magnitude_);
		return *this;
	}

	// Operations
	auto euclidean_vector::operator[](int index) const noexcept -> double {
		return magnitude_.get()[index];
	};

	auto euclidean_vector::operator[](int index) noexcept -> double& {
		return magnitude_.get()[index];
	};

	auto euclidean_vector::operator+() const noexcept -> euclidean_vector {
		return *this;
	};

	auto euclidean_vector::operator-() const noexcept -> euclidean_vector {
		auto copy = *this;
		std::transform(magnitude_.get(),
		               magnitude_.get() + dimensions_,
		               copy.magnitude_.get(),
		               [](auto& val) { return -val; });
		return copy;
	};

	auto euclidean_vector::operator+=(euclidean_vector const& other) -> euclidean_vector& {
		check_dimensions_equal(*this, other);
		std::transform(magnitude_.get(),
		               magnitude_.get() + dimensions_,
		               other.magnitude_.get(),
		               magnitude_.get(),
		               [](auto& val1, auto& val2) { return val1 += val2; });
		return *this;
	};

	auto euclidean_vector::operator-=(euclidean_vector const& other) -> euclidean_vector& {
		check_dimensions_equal(*this, other);
		std::transform(magnitude_.get(),
		               magnitude_.get() + dimensions_,
		               other.magnitude_.get(),
		               magnitude_.get(),
		               [](auto& val1, auto& val2) { return val1 -= val2; });
		return *this;
	};

	auto euclidean_vector::operator*=(double scalar) noexcept -> euclidean_vector& {
		std::transform(magnitude_.get(),
		               magnitude_.get() + dimensions_,
		               magnitude_.get(),
		               [&scalar](auto& val) { return val *= scalar; });
		return *this;
	};

	auto euclidean_vector::operator/=(double scalar) -> euclidean_vector& {
		if (scalar == 0) {
			throw euclidean_vector_error("Invalid vector division by 0");
		}
		std::transform(magnitude_.get(),
		               magnitude_.get() + dimensions_,
		               magnitude_.get(),
		               [&scalar](auto& val) { return val /= scalar; });
		return *this;
	};

	euclidean_vector::operator std::vector<double>() const noexcept {
		return std::vector<double>(magnitude_.get(), magnitude_.get() + dimensions_);
	};

	euclidean_vector::operator std::list<double>() const noexcept {
		return std::list<double>(magnitude_.get(), magnitude_.get() + dimensions_);
	};

	// Member functions
	[[nodiscard]] auto euclidean_vector::at(int index) const -> double {
		check_index_valid(*this, index);
		return magnitude_.get()[index];
	};

	auto euclidean_vector::at(int index) -> double& {
		check_index_valid(*this, index);
		return magnitude_.get()[index];
	};

	[[nodiscard]] auto euclidean_vector::dimensions() const noexcept -> int {
		return dimensions_;
	};

	// Utility functions
	auto euclidean_norm(euclidean_vector const& v) noexcept -> double {
		return v.dimensions() == 0 ? 0 : std::sqrt(euclidean_inner_product(v, v));
	};

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions "
			                             "does not have a unit vector");
		};
		auto norm = euclidean_norm(v);
		if (norm == 0) {
			throw euclidean_vector_error("euclidean_vector with zero euclidean "
			                             "normal does not have a unit vector");
		};
		return v / euclidean_norm(v);
	};

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		check_dimensions_equal(x, y);
		return euclidean_inner_product(x, y);
	};

	// helper functions
	auto check_dimensions_equal(euclidean_vector const& vec1, euclidean_vector const& vec2) -> void {
		if (vec1.dimensions() != vec2.dimensions()) {
			throw euclidean_vector_error("Dimensions of LHS(" + std::to_string(vec1.dimensions())
			                             + ") and RHS(" + std::to_string(vec2.dimensions())
			                             + ") do not match");
		}
	};

	auto check_index_valid(euclidean_vector const& vec, int index) -> void {
		if (index < 0 or index >= vec.dimensions()) {
			throw euclidean_vector_error("Index " + std::to_string(index)
			                             + " is not valid for this "
			                               "euclidean_vector object");
		};
	};
} // namespace comp6771
