#include <iostream>
using namespace std;

class BankAcc {
protected:
    int AccountNumber;
    char accountHolderName[50];
    double Balanceance;

public:
    BankAcc(int AccNo, const char name[], double Balance) {
        AccountNumber = AccNo;
        int i = 0;
        while (name[i] != '\0' && i < 49) {
            accountHolderName[i] = name[i];
            i++;
        }
        accountHolderName[i] = '\0';
        Balanceance = Balance;
    }

    virtual ~BankAcc() {}

    void deposit(double amount) {
        if (amount > 0) {
            Balanceance += amount;
            cout << "Deposited: " << amount << endl;
            cout << "Total Balanceance: " << Balanceance << endl;
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    virtual void withdraw(double amount) {
        if (amount > 0 && amount <= Balanceance) {
            Balanceance -= amount;
            cout << "Withdrew: " << amount << endl;
            cout << "Total Balanceance: " << Balanceance << endl;
        } else {
            cout << "Insufficient funds or invalid amount." << endl;
        }
    }

    virtual double getBalanceance() {
        return Balanceance;
    }

    virtual void displayAccountInfo() const {
        cout << "Account Number: " << AccountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balanceance: " << Balanceance << endl;
    }

    int getAccountNumber() const {
        return AccountNumber;
    }
};

class SavingsAcc : public BankAcc {
public:
    SavingsAcc(int AccNo, const char name[], double Balance)
        : BankAcc(AccNo, name, Balance) {}

    void displayAccountInfo() const override {
        BankAcc::displayAccountInfo();
        cout << "Account Type: Savings" << endl;
    }
};

class CheckingAcc : public BankAcc {
private:
    double overdraftLimit;

public:
    CheckingAcc(int AccNo, const char name[], double Balance, double limit)
        : BankAcc(AccNo, name, Balance), overdraftLimit(limit) {}

    void withdraw(double amount) override {
        if (amount > 0 && amount <= Balanceance + overdraftLimit) {
            Balanceance -= amount;
            cout << "Withdrew: " << amount << endl;
            cout << "Total Balanceance: " << Balanceance << endl;
        } else {
            cout << "Withdrawal exceeds overdraft limit." << endl;
        }
    }

    void displayAccountInfo() const override {
        BankAcc::displayAccountInfo();
        cout << "Account Type: Checking" << endl;
        cout << "Overdraft Limit: " << overdraftLimit << endl;
    }
};

class FixedDepositAccount : public BankAcc {
private:
    int term;
    double interestRate;
    bool interestAdded;

public:
    FixedDepositAccount(int AccNo, const char name[], double Balance, int t)
        : BankAcc(AccNo, name, Balance), term(t), interestRate(7.0), interestAdded(false) {}

    double calculateInterest() const {
        return Balanceance * (interestRate / 100) * (term / 12.0);
    }

    double getBalanceance() override {
        if (!interestAdded) {
            Balanceance += calculateInterest();
            interestAdded = true;
        }
        return Balanceance;
    }

    void displayAccountInfo() const override {
        BankAcc::displayAccountInfo();
        cout << "Account Type: Fixed Deposit" << endl;
        cout << "Term: " << term << " months" << endl;
        cout << "Interest Rate: " << interestRate << "%" << endl;
        cout << "Interest Earned: " << calculateInterest() << endl;
    }
};

class BankingSystem {
private:
    BankAcc* accounts[100];
    int accountCount;

public:
    BankingSystem() {
        accountCount = 0;
    }

    ~BankingSystem() {
        for (int i = 0; i < accountCount; i++) {
            delete accounts[i];
        }
    }

    void createAccount() {
        if (accountCount >= 100) {
            cout << "Cannot create more accounts." << endl;
            return;
        }

        int choice;
        cout << endl << "Select Account Type:" << endl;
        cout << "1. Savings" << endl;
        cout << "2. Checking" << endl;
        cout << "3. Fixed Deposit" << endl;
        cout << "Choice: ";
        cin >> choice;

        int AccNo;
        char name[50];
        double Balanceance;

        cout << "Enter Account Number: ";
        cin >> AccNo;
        

        cout << "Enter Account Holder Name (no spaces): ";
        cin >> name;

        cout << "Enter Initial Balanceance: ";
        cin >> Balanceance;

        if (choice == 1) {
            accounts[accountCount++] = new SavingsAcc(AccNo, name, Balanceance);
        } else if (choice == 2) {
            double limit;
            cout << "Enter Overdraft Limit: ";
            cin >> limit;
            accounts[accountCount++] = new CheckingAcc(AccNo, name, Balanceance, limit);
        } else if (choice == 3) {
            int term;
            cout << "Enter Term (in months): ";
            cin >> term;
            accounts[accountCount++] = new FixedDepositAccount(AccNo, name, Balanceance, term);
        } else {
            cout << "Invalid choice." << endl;
        }
    }

    BankAcc* findAccount(int AccNo) {
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i]->getAccountNumber() == AccNo) {
                return accounts[i];
            }
        }
        return nullptr;
    }

    void depositToAccount() {
        int AccNo;
        double amount;

        cout << "Enter Account Number: ";
        cin >> AccNo;

        BankAcc* acc = findAccount(AccNo);

        if (acc) {
            cout << "Enter Amount to Deposit: ";
            cin >> amount;
            acc->deposit(amount);
        } else {
            cout << "Account not found." << endl;
        }
    }

    void withdrawFromAccount() {
        int AccNo;
        double amount;

        cout << "Enter Account Number: ";
        cin >> AccNo;

        BankAcc* acc = findAccount(AccNo);

        if (acc) {
            cout << "Enter Amount to Withdraw: ";
            cin >> amount;
            acc->withdraw(amount);
        } else {
            cout << "Account not found." << endl;
        }
    }

    void checkBalanceance() {
        int AccNo;
        cout << "Enter Account Number: ";
        cin >> AccNo;

        BankAcc* acc = findAccount(AccNo);

        if (acc) {
            cout << "Current Balanceance: " << acc->getBalanceance() << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

    void displayAccountInfo() {
        int AccNo;
        cout << "Enter Account Number: ";
        cin >> AccNo;

        BankAcc* acc = findAccount(AccNo);

        if (acc) {
            acc->displayAccountInfo();
        } else {
            cout << "Account not found." << endl;
        }
    }
};

int main() {
    BankingSystem bank;
    int choice;

    do {
        cout << endl << "===== BANKING SYSTEM MENU =====" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Check Balanceance" << endl;
        cout << "5. Display Account Info" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << "==============================" << endl;

        switch (choice) {
        case 1:
            bank.createAccount();
            break;
        case 2:
            bank.depositToAccount();
            break;
        case 3:
            bank.withdrawFromAccount();
            break;
        case 4:
            bank.checkBalanceance();
            break;
        case 5:
            bank.displayAccountInfo();
            break;
        case 6:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 6);

    return 0;
}
