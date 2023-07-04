#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
// Custom Header file
#include "OutputHandler.h"
#include "Member.h"
using namespace std;

// functions prototypes
void display_menu_and_actions();
void printProgressBar(int progress, int total, int barWidth);
Member* create_new_member_based_on_credit(std::string id, std::string name, int credit);
int get_id_number(const std::string& input); 
void read_members();
void add_members();
void edit_members();
void remove_members();
bool isValidEditOption(int action);

// global variables
OutputHandler outputhandler;
std::vector<Member*> members;
int last_id = 0;
string FILE_NAME = "./MEMBER.txt";

// Inheritance
class SilverMember : public Member {
public:
    SilverMember() {
        level = "Silver";
    }

    // Polymorphism
    void DisplayAttributes() override {
        outputhandler.print_c("ID: " + GetId() + "\nName: " + GetName() + "\nLevel: " + level + "\nCredit: " + std::to_string(credit) + "\n-------------------\n", outputhandler.SILVER, "1");
    }
};

// Inheritance
class GoldMember : public Member {
public:
    GoldMember() {
        level = "Gold";
    }

    // Polymorphism
    void DisplayAttributes() override {
        outputhandler.print_c("ID: " + GetId() + "\nName: " + GetName() + "\nLevel: " + level + "\nCredit: " + std::to_string(credit) + "\n-------------------\n", outputhandler.GOLD);
    }
};

// Enum to store user input
enum Action {
    Display = 1,
    Modify = 2,
    Remove = 3,
    AddNew = 4,
    Exit = 0
};

// Enum to determine the field to edit
enum EditAction {
    Name = 1,
    Credit = 2
};

// Enum of Memberlevel to credit amount
enum MemberLevel {
    Ordinary = 0,
    Silver = 100,
    Gold = 500
};


int main() {
    outputhandler.print_c("Welcome to MEMBER CLI Program.\nDeveloped by Min Oak Kyaw.\n", outputhandler.CYAN);

    int total = 20;
    int progress = 0;
    int barWidth = 30;

    while (progress <= total) {
        printProgressBar(progress, total, barWidth);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        progress++;
    }

    outputhandler.print_c("\n");
    
    // reading and storing member data on program starts up
    read_members();

    // calling main menu function
    display_menu_and_actions();
    return 0;
}

void printProgressBar(int progress, int total, int barWidth) {
    float percentage = static_cast<float>(progress) / total;
    int filled_width = static_cast<int>(barWidth * percentage);

    std::cout << " ";

    if (progress == total) std::cout << "Loaded. "; else std::cout << "Loading ";

    for (int i = 0; i < barWidth; ++i) {
        if (i < filled_width) {
            outputhandler.print_c("=", outputhandler.GREEN);
        }
        else {
            outputhandler.print_c("-", outputhandler.DARK_GREY);
        }
    }
    std::cout << "\r";
    std::cout.flush();
}

void display_menu_and_actions() {
    int action;

    outputhandler.print_c("Available Options:\n1 - Display\n2 - Edit\n3 - Remove\n4 - Add New Member\n0 - Exit\nAction: ");
    cin >> action;

    if (std::cin.fail()) {
        // Clear the error flag and discard any remaining input
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        action = -1;
    }

    // handling inputs
    if (action == Display) {
        read_members();
        for (auto member : members) {
            outputhandler.print_c("\n");
            member->DisplayAttributes();
        }
        outputhandler.print_c("\n");
    }
    else if (action == AddNew) {
        add_members();
    }
    else if (action == Modify) {
        edit_members();
    }
    else if (action == Remove) {
        remove_members();
    }
    else if (action == Exit) {
        outputhandler.print_c("Exiting the program now.\n", outputhandler.BROWN);

        for (const auto& member : members) {
            delete member;
        }

        exit(0);
    }
    else {
        outputhandler.error("Invalid input! Only numbers are allowed.");
    }

    // calling the menu recursively
    display_menu_and_actions();
}

void read_members() {
    ifstream FILE(FILE_NAME);
    if (!FILE) {
        std::cout << "Error opening file: " << FILE_NAME << std::endl;
        return;
    }

    if (!members.empty()) members.clear();

    string line;
    
    // using try catch to catch any errors
    // if there is something wrong with file format
    // since the file is stored and read based on certain formats
    try {
        while (std::getline(FILE, line)) {
            // skipping if the line is empty.
            if (!line.empty()) {
                istringstream iss(line);
                string name;
                string id;
                string level;
                int credit;

                iss >> name >> id >> level >> credit;

                int current_id_number = get_id_number(id);
                if (last_id < current_id_number) {
                    last_id = current_id_number;
                }

                members.push_back(create_new_member_based_on_credit(id, name, credit));
            }
        }
    }
    catch(...) {
        outputhandler.error("Reading file.");
    }
    

    FILE.close();
}

void add_members() {
    std::fstream file(FILE_NAME, ios::app);

    if (!file.is_open()) {
        outputhandler.error("Failed to open the file.");
        return;
    }

    string name, new_line;
    int credit = -1;
    outputhandler.print_c("Name: ");
    cin >> name;

    outputhandler.print_c("The level of a member is determined by the amount of credit.\nThe credit levels are as follow:\n");

    string ordinary_member_credit_level = "|    Between 0 and 99 is considerd as Ordinary Member.   |\n";
    string silver_member_credit_level = "|    Between 100 and 499 is considerd as Silver Member.  |\n";
    string gold_member_credit_level = "|    Above 499 is considerd as Gold Member.              |\n";

    // printing upper enclosure
    for (int i = 1; i < ordinary_member_credit_level.length(); i++) cout << "-";
    cout << std::endl;

    outputhandler.print_c(ordinary_member_credit_level);
    outputhandler.print_c(silver_member_credit_level);
    outputhandler.print_c(gold_member_credit_level);

    // printing lower enclosure
    for (int i = 1; i < ordinary_member_credit_level.length(); i++) cout << "-";
    cout << std::endl;

    while (credit < 0) {
        outputhandler.print_c("Credit: ");
        cin >> credit;

        // checking if the input is int or not
        if (std::cin.fail() || credit < 0) {
            // Clear the error flag and discard any remaining input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            credit = -1;
            outputhandler.error("Please enter a valid input.");
        }
    }

    last_id += 1;
    string id = "TM" + to_string(last_id);

    Member* new_member = create_new_member_based_on_credit(id, name, credit);
    new_member->SetEntryLine();
    file << new_member->entry_line;

    // Closing File
    file.close();
    outputhandler.print_c("- New member created successfully.\n\n", outputhandler.GREEN);
}

void edit_members() {
    ofstream FILE(FILE_NAME, ofstream::out);
    Member* FoundMember = nullptr;
    string search_term;

    outputhandler.print_c("Search: ");
    cin >> search_term;

    // looping through the members vector to find
    for (const auto& member : members) {
        // comparing the search term to the member name
        if (member->GetName() == search_term) {
            // if the member name is the same as the search term
            // store the member inside the found member
            FoundMember = member;
            // break after the first occurance of the member is found
            break;
        }
        else if (member->GetId() == search_term) {
            // if the member id is the same as the search term
            // store the member inside the found member
            FoundMember = member;
            // break after the first occurance of the member is found
            break;
        }
    }

    if (!FoundMember) {
        outputhandler.error("No data found.");
        return;
    }

    outputhandler.print_c("\nFound the following data.\n", outputhandler.GREEN);
    FoundMember->DisplayAttributes();

    int action;

    while (true) {
        outputhandler.print_c("Choose Edit Field. Available Options:\n1 - Name\n2 - Credit\nEnter: ");
        cin >> action;

        if (std::cin.fail() or !isValidEditOption(action)) {
            // Clear the error flag and discard any remaining input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            outputhandler.error("Please enter a valid input.");
        }
        else {
            break;
        }
    }

    string temp;
    switch (action) {
        case EditAction::Name:
            outputhandler.print_c("Name: ");
            cin >> temp;
            FoundMember->SetName(temp);
            break;
        case EditAction::Credit:
            outputhandler.print_c("The level of a member is determined by the amount of credit.\nThe credit levels are as follow:\n");

            string ordinary_member_credit_level = "|    Between 0 and 99 is considerd as Ordinary Member.   |\n";
            string silver_member_credit_level = "|    Between 100 and 499 is considerd as Silver Member.  |\n";
            string gold_member_credit_level = "|    Above 499 is considerd as Gold Member.              |\n";

            // printing upper enclosure
            for (int i = 1; i < ordinary_member_credit_level.length(); i++) cout << "-";
            cout << std::endl;

            outputhandler.print_c(ordinary_member_credit_level);
            outputhandler.print_c(silver_member_credit_level);
            outputhandler.print_c(gold_member_credit_level);

            // printing lower enclosure
            for (int i = 1; i < ordinary_member_credit_level.length(); i++) cout << "-";
            cout << std::endl;

            outputhandler.print_c("Credit: ");
            cin >> FoundMember->credit;
            break;
    }

    FoundMember = create_new_member_based_on_credit(FoundMember->GetId(), FoundMember->GetName(), FoundMember->credit);

    // updating members vector
    for (const auto& member : members) {
        if (member->GetId() == FoundMember->GetId()) {
            member->SetName(FoundMember->GetName());
            member->credit = FoundMember->credit;
            member->level = FoundMember->level;
            break;
        }
    }

    free(FoundMember);

    string updated_members_string = "";

    for (const auto& member : members) {
        member->SetEntryLine();
        updated_members_string += member->entry_line;
    }

    if (FILE.is_open()) {
        FILE << updated_members_string;
        outputhandler.print_c("- Updated successfully.\n\n", outputhandler.GREEN);
    }
    else {
        outputhandler.error("Failed to open the file.");
    }

    return;
}

void remove_members() {
    ofstream FILE(FILE_NAME, ofstream::out);
    string search_term;

    outputhandler.print_c("Search by ID: ");
    cin >> search_term;

    // looping through the members vector to find
    for (const auto& member : members) {
        if (member->GetId() == search_term) {
            int index = distance(members.begin(), find(members.begin(), members.end(), member));
            members.erase(members.begin() + index);
            break;
        }
    }

    string updated_members_string = "";

    for (const auto& member : members) {
        member->SetEntryLine();
        updated_members_string += member->entry_line;
    }

    if (FILE.is_open()) {
        FILE << updated_members_string;
        outputhandler.print_c("- Removed successfully.\n\n", outputhandler.GREEN);
    }
    else {
        outputhandler.error("Failed to open the file.");
    }

    return;
}

Member* create_new_member_based_on_credit(std::string id, std::string name, int credit) {
    if (credit >= Gold) {
        GoldMember* gold_member = new GoldMember();
        gold_member->SetId(id);
        gold_member->SetName(name);
        gold_member->credit = credit;
        return gold_member;
    }
    else if (credit >= Silver) {
        SilverMember* silver_member = new SilverMember();
        silver_member->SetId(id);
        silver_member->SetName(name);
        silver_member->credit = credit;
        return silver_member;
    }
    else {
        Member* member = new Member();
        member->SetId(id);
        member->SetName(name);
        member->credit = credit;
        return member;
    }
}

int get_id_number(const std::string& input) {
    std::string numbers;

    for (char c : input) {
        if (std::isdigit(c)) {
            numbers += c;
        }
    }
    return std::stoi(numbers);
}

bool isValidEditOption(int action) {
    switch (action) {
    case EditAction::Name:
    case EditAction::Credit:
        return true;
    default:
        return false;
    }
}