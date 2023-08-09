#include <iostream>
#include <sstream>
#include "primitive_types.h"

class PrettyPrint {
  public:
	static const string RED;
	static const string GREEN;
	static const string YELLOW;
	static const string BLUE;
	static const string MAGENTA;
	static const string CYAN;
	static const string RESET;

	PrettyPrint(){};
	~PrettyPrint() {
		std::cout << RESET;
	};

	static void set_color(const string &color) {
		if(color == "red" || color == RED) {
			std::cout << RED;
		} else if(color == "green" || color == GREEN) {
			std::cout << GREEN;
		} else if(color == "yellow" || color == YELLOW) {
			std::cout << YELLOW;
		} else if(color == "blue" || color == BLUE) {
			std::cout << BLUE;
		} else if(color == "magenta" || color == MAGENTA) {
			std::cout << MAGENTA;
		} else if(color == "cyan" || color == CYAN) {
			std::cout << CYAN;
		} else if(color == "reset" || color == RESET) {
			std::cout << RESET;
		} else {
			std::cerr << "Invalid color: " << color << std::endl;
		}
	}

	template<typename... Args>
	static void print(const Args &...args) {
		std::ostringstream oss;
		(oss << ... << args);
		std::cout << oss.str();
	}
};

const string PrettyPrint::RED	   = "\033[31m";
const string PrettyPrint::GREEN   = "\033[32m";
const string PrettyPrint::YELLOW  = "\033[33m";
const string PrettyPrint::BLUE	   = "\033[34m";
const string PrettyPrint::MAGENTA = "\033[35m";
const string PrettyPrint::CYAN	   = "\033[36m";
const string PrettyPrint::RESET   = "\033[0m";