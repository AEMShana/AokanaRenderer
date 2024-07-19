#pragma once
#include <stdexcept>
#include <exception>
#include <string>

namespace Aokana {

	inline bool CHECK(bool x) {
		if (x) return true;
		throw std::runtime_error(std::string("[Check Failed] [File]") + __FILE__ + " [LINE] " + std::to_string(x) + '\n');
	}

}