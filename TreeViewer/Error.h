#pragma once
#include <string>

struct Error {
	std::string source = "";
	std::string message = "";

	Error(std::string source, std::string message) {
		this->source = source;
		this->message = message;
		return;
	}
};
