#pragma once
#include <string>

template <typename T1, typename T2, typename T3>
struct tuple {
    tuple(const T1& first, const T2& second, const T3& third) {
        this->first = first;
        this->second = second;
        this->third = third;
    }
    T1 first;
    T2 second;
    T3 third;
};

struct Error {
	std::string source = "";
	std::string message = "";

	Error(std::string source, std::string message) {
		this->source = source;
		this->message = message;
		return;
	}
};
