#include "Member.h"
#include <iostream>
#include <string>
#include <fstream>

Member::Member() {
    level = "Ordinary";
}

void Member::DisplayAttributes() {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Level: " << level << std::endl;
    std::cout << "Credit: " << credit << std::endl;
    std::cout << "-------------------\n";
}

std::string Member::GetName() {
    return name;
}

void Member::SetName(std::string Name) {
    name = Name;
}

std::string Member::GetId() {
    return id;
}

void Member::SetId(std::string ID) {
    id = ID;
}

void Member::SetEntryLine() {
    entry_line = name + " " + id + " " + level + " " + std::to_string(credit) + "\n";
}