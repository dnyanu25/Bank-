#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Account {
    int id;
    string name;
    double balance;

public:
    Account(int id, string name, double balance) : id(id), name(name), balance(balance) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    void display() const {
        cout << "ID: " << id << " | Name: " << name << " | Balance: " << balance << endl;
    }

    string serialize() const {
        return to_string(id) + "," + name + "," + to_string(balance) + "\n";
    }

    static Account deserialize(const string& line) {
        int id;
        string name;
        double balance;
        size_t pos1 = line.find(",");
        size_t pos2 = line.rfind(",");
        id = stoi(line.substr(0, pos1));
        name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        balance = stod(line.substr(pos2 + 1));
        return Account(id, name, balance);
    }
};

class Bank {
    vector<Account> accounts;
    const string filename = "accounts.txt";

public:
    Bank() {
        loadAccounts();
    }

    void createAccount(int id, string name, double initialDeposit) {
        accounts.emplace_back(id, name, initialDeposit);
        saveAccounts();
    }

    void deposit(int id, double amount) {
        for (auto &acc : accounts) {
            if (acc.getId() == id) {
                acc.deposit(amount);
                saveAccounts();
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void withdraw(int id, double amount) {
        for (auto &acc : accounts) {
            if (acc.getId() == id) {
                if (acc.withdraw(amount)) {
                    cout << "Withdrawal successful!\n";
                } else {
                    cout << "Insufficient funds!\n";
                }
                saveAccounts();
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void displayAll() {
        for (const auto &acc : accounts) {
            acc.display();
        }
    }

private:
    void saveAccounts() {
        ofstream file(filename);
        for (const auto &acc : accounts) {
            file << acc.serialize();
        }
    }

    void loadAccounts() {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                accounts.push_back(Account::deserialize(line));
            }
        }
    }
};

int main() {
    Bank bank;
    int choice;

    do {
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Display All Accounts\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string name;
            double deposit;
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Name: "; cin >> name;
            cout << "Enter Initial Deposit: "; cin >> deposit;
            bank.createAccount(id, name, deposit);
        } else if (choice == 2) {
            int id;
            double amount;
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Amount: "; cin >> amount;
            bank.deposit(id, amount);
        } else if (choice == 3) {
            int id;
            double amount;
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Amount: "; cin >> amount;
            bank.withdraw(id, amount);
        } else if (choice == 4) {
            bank.displayAll();
        }
    } while (choice != 5);

    return 0;
}
