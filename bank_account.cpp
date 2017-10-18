#include "bank_account.h"
#include <iostream>
#include <cstring>
#include <stdio.h>
#include "names.h"
#include "mnb.h"
#include <ctime> // srand

//#define  __USE_MINGW_ANSI_STDIO 1

using namespace std;

template<typename T, size_t N>
size_t Utils::size_of_array(const T(&)[N])
{
    return N;
}

std::array<bank_descr_t, 6> BankAccountHelper::banks_in_system =
{
    bank_descr_t(BANK_BUDAPEST,   BUDAPEST_BANK_PREFIX),
    bank_descr_t(BANK_CITI,       CITI_BANK_PREFIX),
    bank_descr_t(BANK_OTP,        OTP_BANK_PREFIX),
    bank_descr_t(BANK_GRANIT,     GRANIT_BANK_PREFIX),
    bank_descr_t(BANK_ERSTE,      ERSTE_BANK_PREFIX),
    bank_descr_t(BANK_RAIFFEISEN, RAIFFEISEN_BANK_PREFIX) // todo
};


BankAccountHelper::BankAccountHelper(){ srand(/*static_cast <unsigned int>*/ (time(NULL))); };

void BankAccountHelper::initialize_banks()
{
    cout << "LOG: BankAccountHelper::initialize_banks(): ENTER" << endl;

    std::lock_guard<std::mutex> guard(banks_map_mutex);

    banks_map.clear();

    for(const auto item : banks_in_system)
    {
        banks_map.emplace(item.name, Bank(item.name, item.prefix)) ;
    }

    cout << "LOG: BankAccountHelper::initialize_banks(): EXIT" << endl;
}

bool BankAccountHelper::generate_accounts(const size_t number_of_accounts_by_bank)
{
    cout << "LOG: BankAccountHelper::generate_accounts(public): ENTER" << endl;

    for(auto& item : banks_map)
    {
        cout << item.second.get_name() << endl;
        generate_accounts(item.second, number_of_accounts_by_bank);
    }

    cout << "LOG: BankAccountHelper::generate_accounts(public): EXIT" << endl;
}

bool BankAccountHelper::generate_accounts(Bank& bank, const size_t number_of_accounts)
{
    cout << "LOG: BankAccountHelper::generate_accounts(): ENTER" << endl;

    // output = min + (rand() % static_cast<int>(max - min + 1));
    const unsigned int half = number_of_accounts / 2;
    for( unsigned int i = 0; i < number_of_accounts ; ++i  )
    {
        string username;
        if(i < half)
        {
            username = generate_account_name(true);
        }
        else
        {
            username = generate_account_name(false);
        }

        string szlaszam = bank.create_account_by_name(username); // todo 1234

        cout << username << ": " << szlaszam << endl;
    }

    cout << "LOG: BankAccountHelper::generate_accounts(): EXIT" << endl;
    return true;
}

std::string/*&&*/ BankAccountHelper::generate_account_nr(const std::string& prefix) noexcept
{
    int number = rand() % 100000000;

//cout << "generate_account_nr(): " << number << endl;

    char postfix [33]; postfix[0] = '\0';
    snprintf(postfix, sizeof(postfix), "%08d", number);
//cout << "generate_account_nr(): " <<  string(prefix) + "-" + postfix<< endl;
    return /*std::move(*/string(prefix) + "-" + postfix/*)*/;
}

string/*&&*/ BankAccountHelper::generate_account_name(const bool man) noexcept
{
    if(man) return surnames_arr[rand() % surnames_arr.size()] + " " + men_lastnames_arr[rand() % men_lastnames_arr.size() ];
    else    return surnames_arr[rand() % surnames_arr.size()] + " " + women_lastnames_arr[rand() % women_lastnames_arr.size() ];
}

/*
bool BankAccountHelper::register_account_nr (const string& bank_name, const string& acc_name, const std::string& acc_nr)
{
    // todo;
    try
    {
        Bank& bank = banks_map.at(bank_name);
        bank.accounts_map.emplace(std::make_pair<std::string, BankAccount>(acc_name, BankAccount(acc_name, acc_nr));
    }catch(const std::exception& ex)
    {
        cout << "LOG_ERR: BankAccountHelper::register_account_nr(): EXCEPTION: " << ex.what() << endl;
        return false;
    }

    return true;
}
*/
/*bool BankAccountHelper::unregister_account_nr(const char[] bank, const std::string& acc_nr)
{
    return true;
}*/


BankAccount::BankAccount(const string& user_name, const string& account_nr) : acc_name(user_name), acc_no(account_nr)
{
}

BankAccount::BankAccount(BankAccount&& rhs)
: acc_name(std::move(rhs.acc_name)),
  acc_no(std::move(rhs.acc_no))
{

}

Bank::Bank(Bank&& rhs)
: bank_name(std::move(rhs.bank_name)),
  account_nr_prefix(std::move(rhs.account_nr_prefix)),
  accounts_map(std::move(rhs.accounts_map))
{
}


const string& BankAccount::get_account_number() const noexcept
{
    return acc_no;
}

const string& BankAccount::get_account_name() const noexcept
{
    return acc_name;
}
/*
ostream& BankAccount::operator<<(ostream& os, const BankAccount& ba)
{
    os << "BankAccount.account_name: " << ba.acc_name << endl;
    os << "BankAccount.account_nr: " << ba.acc_no << endl;
    return os;
}
*/
ostream& BankAccount::operator<<(ostream& os)
{
    os << "BankAccount.account_name: " << acc_name << endl;
    os << "BankAccount.account_nr: " << acc_no << endl;
    return os;
}


bool BankAccount::operator<(const BankAccount& rhs) const
{
    std::string left_cpy(this->acc_no), right_cpy(rhs.acc_no);

    return left_cpy < right_cpy;
}



Bank::Bank(const std::string& name, const std::string& account_prefix) : bank_name(name), account_nr_prefix(account_prefix)
{
    cout << "Bank::Bank(): " << bank_name << " is created with prefix: " << account_nr_prefix << endl;
}

std::string Bank::create_account_by_name(const std::string& username)
{
    std::string internal_account_nr = BankAccountHelper::generate_account_nr(get_prefix());

    while(1)
    {
        internal_account_nr = BankAccountHelper::generate_account_nr(get_prefix());

        // todo lock account_nrs

        if(!account_nr_exists(internal_account_nr))
        {
            std::lock_guard<std::mutex> guard(account_nrs_mutex);
            std::lock_guard<std::mutex> guard2(accounts_map_mutex);

            account_nrs.insert(internal_account_nr);
            accounts_map.insert(std::pair<string, BankAccount>(username, BankAccount( username, internal_account_nr)));

            return internal_account_nr;
        }
    }

}

const bool Bank::account_nr_exists(std::string& account_nr) noexcept
{
    auto iter = account_nrs.find(account_nr);
    if(iter != account_nrs.end())
    {
        return true;
    }
    else
        return false;
}

const std::string& Bank::get_prefix() const
{
    return this->account_nr_prefix;
}

const std::string& Bank::get_name() const
{
    return this->bank_name;
}

bool Bank::operator<(const Bank& rhs) const
{
    std::string left_cpy(this->bank_name), right_cpy(rhs.bank_name);

    std::transform(left_cpy.begin(), left_cpy.end(), left_cpy.begin(), ::tolower);
    std::transform(right_cpy.begin(), right_cpy.end(), right_cpy.begin(), ::tolower);

    return left_cpy < right_cpy;
}
