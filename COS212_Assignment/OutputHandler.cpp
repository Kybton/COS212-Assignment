#include "OutputHandler.h"
#include <iostream>
#include <string>

void OutputHandler::print(std::string desired_text) {
	std::cout << desired_text;
}

void OutputHandler::error(std::string desired_text) {
	std::cout << "\n\033[1;" << RED << "m" << "! ERROR: " << "\033[0m" << desired_text << "\n\n";
}

void OutputHandler::print_c(std::string desired_text, Colors color, std::string status) {
	std::cout << "\033[" << status << ";" << color << "m" << desired_text << "\033[0m";
}