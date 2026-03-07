#include "error.h"


const int ERR_ARR_COUNT = core::ErrorType::Clear + 1;
std::string ErrorArr[ERR_ARR_COUNT] = {};

namespace core {
void set_errors() {
	std::string prefix = "Error: ";
	char suffix = '!';

	ErrorArr[ErrorType::Nullptr] = "Object is a null ptr";
	ErrorArr[ErrorType::WidgetFoundAgain] = "Widget was already found";
	ErrorArr[ErrorType::FileOpenFail] = "File failed to open";
	ErrorArr[ErrorType::SubwinNotFound] = "Subwin could not be found";
	ErrorArr[ErrorType::SceneNotFound] = "Scene could not be found";
	ErrorArr[ErrorType::Clear] = "No errors were found";


	for (int i=0; i<ERR_ARR_COUNT; i++) {
		ErrorArr[i].insert(0,prefix);
		ErrorArr[i].push_back(suffix);
	}
}

Error::Error(ErrorType::Codes type) {
	this->_type = type;
}


const std::string& Error::what() {
	return ErrorArr[(int)_type];
}

bool Error::exists() {
	if (_type == ErrorType::Clear) {
		return false;
	}

	return true;
}
}
