#include <iostream>
#include <fstream>
#include <string>

using namespace std;




// Base class
class Account
{
protected:
    int    accountId;
    string ownerName;
    double balance;

    // Simple bool flags instead of bitwise permissions
    bool canDeposit;
    bool canWithdraw;
    bool canTransfer;

    // Fixed array instead of vector
    double transactions[100];
    int    txnCount;

public:
    virtual void deposit(double amount)  = 0;
    virtual void withdraw(double amount) = 0;
    virtual void showAccount()           = 0;
    virtual void saveToFile(ofstream& f) = 0;

    void showHistory()
    {
        if (txnCount == 0)
        {
            cout << "  No transactions yet.\n";
            return;
        }
        cout << "\n  --- Transaction History ---\n";
        for (int i = 0; i < txnCount; i++)
        {
            if (transactions[i] > 0)
                cout << "  [" << i+1 << "] Deposit  : +Rs " << transactions[i] << "\n";
            else
                cout << "  [" << i+1 << "] Withdraw : -Rs " << -transactions[i] << "\n";
        }
    }

    int    getId()      { return accountId; }
    string getName()    { return ownerName; }
    double getBalance() { return balance; }
    int    getTxnCount(){ return txnCount; }

    double getTransaction(int i) { return transactions[i]; }

    void addTransaction(double t)
    {
        if (txnCount < 100)
        {
            transactions[txnCount] = t;
            txnCount++;
        }
    }

    void clearTransactions() { txnCount = 0; }

    // Plain destructor (not virtual)
    ~Account()
    {
        cout << "  [Account of " << ownerName << " deleted]\n";
    }
};


// Savings Account
class SavingsAccount : public Account
{
private:
    double interestRate;

public:
    SavingsAccount(int id, string name, double startBalance)
    {
        accountId    = id;
        ownerName    = name;
        balance      = startBalance;
        interestRate = 0.05;
        txnCount     = 0;

        // Simple bool flags
        canDeposit  = true;
        canWithdraw = true;
        canTransfer = true;
    }

    void deposit(double amount)
    {
        if (!canDeposit)
        {
            cout << "  Deposit not allowed!\n";
            return;
        }
        if (amount <= 0)
        {
            cout << "  Amount must be greater than 0!\n";
            return;
        }
        balance += amount;
        addTransaction(amount);
        cout << "  Rs " << amount << " deposited!\n";
        cout << "  New Balance: Rs " << balance << "\n";
    }

    void withdraw(double amount)
    {
        if (!canWithdraw)
        {
            cout << "  Withdrawal not allowed!\n";
            return;
        }
        if (amount <= 0)
        {
            cout << "  Amount must be greater than 0!\n";
            return;
        }
        if (amount > balance)
        {
            cout << "  Not enough balance! Your balance: Rs " << balance << "\n";
            return;
        }
        balance -= amount;
        addTransaction(-amount);
        cout << "  Rs " << amount << " withdrawn!\n";
        cout << "  New Balance: Rs " << balance << "\n";
    }

    void addInterest()
    {
        double interest = balance * interestRate;
        balance += interest;
        addTransaction(interest);
        cout << "  Interest Added: Rs " << interest << "\n";
        cout << "  New Balance: Rs " << balance << "\n";
    }

    void showAccount()
    {
        cout << "\n  ====== SAVINGS ACCOUNT ======\n";
        cout << "  Account ID   : " << accountId << "\n";
        cout << "  Owner Name   : " << ownerName << "\n";
        cout << "  Balance      : Rs " << balance << "\n";
        cout << "  Interest Rate: " << interestRate * 100 << "%\n";
        cout << "  Permissions  : ";
        if (canDeposit)  cout << "[Deposit] ";
        if (canWithdraw) cout << "[Withdraw] ";
        if (canTransfer) cout << "[Transfer] ";
        cout << "\n";
    }

    void saveToFile(ofstream& f)
    {
        f << "SAVINGS\n" << accountId << "\n" << ownerName << "\n";
        f << balance << "\n";
        f << canDeposit << " " << canWithdraw << " " << canTransfer << "\n";
        f << interestRate << "\n";
        f << txnCount << "\n";
        for (int i = 0; i < txnCount; i++)
            f << transactions[i] << "\n";
    }

    double getInterestRate() { return interestRate; }
};


// Current Account
class CurrentAccount : public Account
{
private:
    double overdraftLimit;

public:
    CurrentAccount(int id, string name, double startBalance)
    {
        accountId      = id;
        ownerName      = name;
        balance        = startBalance;
        overdraftLimit = 10000;
        txnCount       = 0;

        // Simple bool flags
        canDeposit  = true;
        canWithdraw = true;
        canTransfer = true;
    }

    void deposit(double amount)
    {
        if (!canDeposit)
        {
            cout << "  Deposit not allowed!\n";
            return;
        }
        if (amount <= 0)
        {
            cout << "  Amount must be greater than 0!\n";
            return;
        }
        balance += amount;
        addTransaction(amount);
        cout << "  Rs " << amount << " deposited!\n";
        cout << "  New Balance: Rs " << balance << "\n";
    }

    void withdraw(double amount)
    {
        if (!canWithdraw)
        {
            cout << "  Withdrawal not allowed!\n";
            return;
        }
        if (amount <= 0)
        {
            cout << "  Amount must be greater than 0!\n";
            return;
        }
        if (amount > balance + overdraftLimit)
        {
            cout << "  Exceeds overdraft limit!\n";
            cout << "  Max you can withdraw: Rs " << balance + overdraftLimit << "\n";
            return;
        }
        balance -= amount;
        addTransaction(-amount);
        cout << "  Rs " << amount << " withdrawn!\n";
        if (balance < 0)
            cout << "  Warning: Account is in overdraft! Balance: Rs " << balance << "\n";
        else
            cout << "  New Balance: Rs " << balance << "\n";
    }

    void showAccount()
    {
        cout << "\n  ====== CURRENT ACCOUNT ======\n";
        cout << "  Account ID     : " << accountId << "\n";
        cout << "  Owner Name     : " << ownerName << "\n";
        cout << "  Balance        : Rs " << balance << "\n";
        cout << "  Overdraft Limit: Rs " << overdraftLimit << "\n";
        cout << "  Permissions    : ";
        if (canDeposit)  cout << "[Deposit] ";
        if (canWithdraw) cout << "[Withdraw] ";
        if (canTransfer) cout << "[Transfer] ";
        cout << "\n";
    }

    void saveToFile(ofstream& f)
    {
        f << "CURRENT\n" << accountId << "\n" << ownerName << "\n";
        f << balance << "\n";
        f << canDeposit << " " << canWithdraw << " " << canTransfer << "\n";
        f << overdraftLimit << "\n";
        f << txnCount << "\n";
        for (int i = 0; i < txnCount; i++)
            f << transactions[i] << "\n";
    }
};


// Monthly summary using array
void showMonthlySummary(Account* accounts[], int count)
{
    double monthlyNet[12] = {0};
    string months[12] = {"Jan","Feb","Mar","Apr","May","Jun",
                         "Jul","Aug","Sep","Oct","Nov","Dec"};

    for (int i = 0; i < count; i++)
    {
        int txns = accounts[i]->getTxnCount();
        for (int j = 0; j < txns; j++)
        {
            int m = j % 12;
            monthlyNet[m] += accounts[i]->getTransaction(j);
        }
    }

    cout << "\n  ====== MONTHLY SUMMARY ======\n";
    for (int i = 0; i < 12; i++)
        cout << "  " << months[i] << " : Rs " << monthlyNet[i] << "\n";
}


// Save accounts to file
void saveAll(Account* accounts[], int count)
{
    ofstream file("bank_data.txt");
    if (!file)
    {
        cout << "  Could not open file!\n";
        return;
    }
    file << count << "\n";
    for (int i = 0; i < count; i++)
        accounts[i]->saveToFile(file);
    file.close();
    cout << "  Accounts saved to bank_data.txt\n";
}


// Load accounts from file
int loadAll(Account* accounts[], int currentCount)
{
    // Delete existing accounts
    for (int i = 0; i < currentCount; i++)
        delete accounts[i];

    ifstream file("bank_data.txt");
    if (!file)
    {
        cout << "  No saved data found.\n";
        return 0;
    }

    int count;
    file >> count;
    file.ignore();

    int loaded = 0;

    for (int i = 0; i < count; i++)
    {
        string type;
        getline(file, type);

        int    id, txnCount;
        string name;
        double bal, extra;
        bool   dep, with, trans;

        file >> id;
        file.ignore();
        getline(file, name);
        file >> bal >> dep >> with >> trans >> extra >> txnCount;
        file.ignore();

        if (type == "SAVINGS")
        {
            SavingsAccount* sa = new SavingsAccount(id, name, bal);
            sa->clearTransactions();
            for (int j = 0; j < txnCount; j++)
            {
                double t;
                file >> t;
                file.ignore();
                sa->addTransaction(t);
            }
            accounts[loaded] = sa;
            loaded++;
        }
        else if (type == "CURRENT")
        {
            CurrentAccount* ca = new CurrentAccount(id, name, bal);
            ca->clearTransactions();
            for (int j = 0; j < txnCount; j++)
            {
                double t;
                file >> t;
                file.ignore();
                ca->addTransaction(t);
            }
            accounts[loaded] = ca;
            loaded++;
        }
    }

    file.close();
    cout << "  Loaded " << loaded << " account(s) from file.\n";
    return loaded;
}


// Find account by ID
Account* findById(Account* accounts[], int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (accounts[i]->getId() == id)
            return accounts[i];
    }
    return NULL;
}


int main()
{
    Account* accounts[50];
    int accountCount = 0;
    int nextId = 1001;
    int choice;

    cout << "\n  =========================================\n";
    cout << "      WELCOME TO  BANK MANAGEMENT  SYSTEM\n";
    cout << "  =========================================\n";

    do
    {
        cout << "\n  ----------- MAIN MENU -----------\n";
        cout << "  1.  Create New Account\n";
        cout << "  2.  Deposit Money\n";
        cout << "  3.  Withdraw Money\n";
        cout << "  4.  View Account Details\n";
        cout << "  5.  View Transaction History\n";
        cout << "  6.  Add Interest (Savings Only)\n";
        cout << "  7.  Monthly Summary\n";
        cout << "  8.  Save Accounts to File\n";
        cout << "  9.  Load Accounts from File\n";
        cout << "  10. Show All Accounts\n";
        cout << "  0.  Exit\n";
        cout << "  ---------------------------------\n";
        cout << "  Enter choice: ";
        cin  >> choice;

        if (choice == 1)
        {
            if (accountCount >= 50)
            {
                cout << "  Max accounts reached!\n";
            }
            else
            {
                cout << "\n  1. Savings Account\n";
                cout << "  2. Current Account\n";
                cout << "  Choose: ";
                int type; cin >> type;

                string name;
                double startBal;

                cout << "  Enter your name: ";
                cin.ignore();
                getline(cin, name);
                cout << "  Enter opening balance (Rs): ";
                cin >> startBal;

                Account* acc = NULL;

                if (type == 1)
                {
                    acc = new SavingsAccount(nextId, name, startBal);
                    cout << "\n  Savings Account created!\n";
                }
                else if (type == 2)
                {
                    acc = new CurrentAccount(nextId, name, startBal);
                    cout << "\n  Current Account created!\n";
                }
                else
                {
                    cout << "  Invalid choice!\n";
                }

                if (acc != NULL)
                {
                    accounts[accountCount] = acc;
                    accountCount++;
                    cout << "  Your Account ID: " << nextId << "\n";
                    nextId++;
                }
            }
        }

        else if (choice == 2)
        {
            int id; double amount;
            cout << "\n  Enter Account ID: "; cin >> id;
            Account* acc = findById(accounts, accountCount, id);
            if (acc == NULL)
                cout << "  Account not found!\n";
            else
            {
                cout << "  Enter deposit amount (Rs): "; cin >> amount;
                acc->deposit(amount);
            }
        }

        else if (choice == 3)
        {
            int id; double amount;
            cout << "\n  Enter Account ID: "; cin >> id;
            Account* acc = findById(accounts, accountCount, id);
            if (acc == NULL)
                cout << "  Account not found!\n";
            else
            {
                cout << "  Enter withdrawal amount (Rs): "; cin >> amount;
                acc->withdraw(amount);
            }
        }

        else if (choice == 4)
        {
            int id;
            cout << "\n  Enter Account ID: "; cin >> id;
            Account* acc = findById(accounts, accountCount, id);
            if (acc == NULL)
                cout << "  Account not found!\n";
            else
                acc->showAccount();
        }

        else if (choice == 5)
        {
            int id;
            cout << "\n  Enter Account ID: "; cin >> id;
            Account* acc = findById(accounts, accountCount, id);
            if (acc == NULL)
                cout << "  Account not found!\n";
            else
                acc->showHistory();
        }

        else if (choice == 6)
        {
            int id;
            cout << "\n  Enter Savings Account ID: "; cin >> id;
            Account* acc = findById(accounts, accountCount, id);
            if (acc == NULL)
                cout << "  Account not found!\n";
            else
            {
                SavingsAccount* sa = dynamic_cast<SavingsAccount*>(acc);
                if (sa != NULL)
                    sa->addInterest();
                else
                    cout << "  This is not a Savings Account!\n";
            }
        }

        else if (choice == 7)
        {
            if (accountCount == 0)
                cout << "  No accounts exist yet!\n";
            else
                showMonthlySummary(accounts, accountCount);
        }

        else if (choice == 8)
            saveAll(accounts, accountCount);

        else if (choice == 9)
            accountCount = loadAll(accounts, accountCount);

        else if (choice == 10)
        {
            if (accountCount == 0)
                cout << "  No accounts found! :( )\n";
            else
            {
                cout << "\n  ====== ALL ACCOUNTS (" << accountCount << " total) ======\n";
                for (int i = 0; i < accountCount; i++)
                    accounts[i]->showAccount();
            }
        }

        else if (choice != 0)
            cout << "  Invalid choice! Try again. :0\n";

    } while (choice != 0);

    // Free memory
    for (int i = 0; i < accountCount; i++)
        delete accounts[i];

    cout << "\n  Thank you for Bank Management System :)\n";
    cout << "  Goodbye!\n\n";

    return 0;
}

