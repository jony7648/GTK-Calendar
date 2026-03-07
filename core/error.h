#pragma once

#include <iostream>



namespace core::ErrorType {
enum Codes {
	Nullptr = 0,
	WidgetFoundAgain,
	FileOpenFail,
	SubwinNotFound,
	SceneNotFound,
	Clear,
};
}

namespace core {
void set_errors();

class Error {
private:
	ErrorType::Codes _type;

public:
	Error(ErrorType::Codes type);
	const std::string& what();
	bool exists();
};
}
