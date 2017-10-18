#ifndef BANK_ACCOUNT_H_
#define BANK_ACCOUNT_H_

#include <string>
#include <stdexcept>
#include <map>
#include <array>
#include <unordered_set>
#include <locale>
#include <algorithm>

#include <thread>
#include <mutex>
#include <condition_variable>

constexpr const char   BUDAPEST_BANK_PREFIX[] = "10102093-84640001";
constexpr const char       CITI_BANK_PREFIX[] = "10800007-00000000";
constexpr const char        OTP_BANK_PREFIX[] = "11773322-00065000";
constexpr const char     GRANIT_BANK_PREFIX[] = "12100004-15890000";
constexpr const char      ERSTE_BANK_PREFIX[] = "12501041-00010980";
constexpr const char RAIFFEISEN_BANK_PREFIX[] = "12011382-03425000";

constexpr const char BANK_BUDAPEST[]   = "BUDAPEST BANK";
constexpr const char BANK_CITI[]       = "CITIBANK";
constexpr const char BANK_OTP[]        = "OTP BANK";
constexpr const char BANK_GRANIT[]     = "GRANIT BANK";
constexpr const char BANK_ERSTE[]      = "ERSTE BANK";
constexpr const char BANK_RAIFFEISEN[] = "RAIFFEISEN BANK";

class bank_descr_t
{
public:
    std::string name;
    std::string prefix;
    bank_descr_t(const char n[], const char p[]){ name = std::string(n); prefix = std::string(p);}
};


class Bank;

class Utils
{
public:
    template<typename T, size_t N>
    static size_t size_of_array(const T(&)[N]);

};


class BankAccountHelper
{
public:
    static  std::array<bank_descr_t, 6> banks_in_system;

    BankAccountHelper();
    BankAccountHelper( std::map<const char[], Bank> );

    void initialize_banks();

    bool generate_accounts(const size_t number_of_accounts_by_bank);

    static std::string/*&&*/ generate_account_nr (const std::string& prefix) noexcept;
    /*static*/ std::string/*&&*/ generate_account_name(const bool man) noexcept;
    /*static*/ //bool register_account_nr  (const std::string& bank_name, const std::string& acc_name, const std::string& acc_nr);
    // static bool unregister_account_nr(const std::string& acc_nr);
private:
    /*static*/ bool generate_accounts(Bank& bank, const size_t number_of_accounts);

    std::map<std::string, Bank> banks_map;
    std::mutex                  banks_map_mutex;
};


class BankAccount
{
public:
    BankAccount() = delete;
    BankAccount(const std::string& user_name, const std::string& account_nr);
    BankAccount(BankAccount&& rhs);
    ~BankAccount(){};

    const std::string& get_account_number() const noexcept;
    const std::string& get_account_name() const noexcept;
private:
    bool operator< (const BankAccount& rhs) const;
    //friend std::ostream& operator<<(std::ostream& os, const BankAccount& ba);
    std::ostream& operator<<(std::ostream& os);

    std::string acc_name;
    std::string acc_no;
};

class Bank
{
public:
    Bank() = delete;
    Bank(const std::string& name, const std::string& account_prefix);

    Bank(Bank&& rhs);

    std::string create_account_by_name(const std::string& username);
    inline const bool account_nr_exists(std::string& account_nr) noexcept;
    void add_account(std::pair<std::string, BankAccount>&& ba_pair);

    const std::string& get_prefix() const;
    const std::string& get_name() const;

//private:
    bool operator< (const Bank& rhs) const;

    std::string bank_name;
    std::string account_nr_prefix;
    std::map<std::string, BankAccount> accounts_map;
    std::mutex                         accounts_map_mutex;
    std::unordered_set<std::string>    account_nrs;
    std::mutex                         account_nrs_mutex;
};

#endif // BANK_ACCOUNT_H_

