#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <fstream>

class Member {
private:
    std::string id;
    std::string name;

public:
    std::string level;
    std::string entry_line;
    int credit = 0;

    Member();
    
    virtual void DisplayAttributes();

    std::string GetName();
    void SetName(std::string Name);

    std::string GetId();
    void SetId(std::string ID);

    void SetEntryLine();
};

#endif