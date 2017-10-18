#ifndef MNB_H_INCLUDED
#define MNB_H_INCLUDED

#include <unordered_map>
#include <string>
#include <iostream>

class MNB
{
public:
    MNB();
    ~MNB();

private:
    std::unordered_map<std::string, std::string> account_bank_pair; // szlaszam - bank
};

#endif // MNB_H_INCLUDED
