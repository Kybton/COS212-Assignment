#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include <string>

class OutputHandler {
public:
	enum Colors {
		BLACK = 30,
		RED = 31,
		GREEN = 32,
		BROWN = 33,
		BLUE = 34,
		SILVER = 37,
		DARK_GREY = 90,
		CYAN = 36,
		GOLD = 93,
		WHITE = 97,
	};
	void print(std::string desired_text);
	void error(std::string desired_text);
	void print_c(std::string desired_text, Colors color = WHITE, std::string status = "0");
};

#endif