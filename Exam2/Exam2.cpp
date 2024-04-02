#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <ctime>

struct ExpenseCategory {
    std::string name;
    double totalAmount;
};

class Wallet {
private:
    double balance;

public:
    Wallet() : balance(0.0) {}

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() const {
        return balance;
    }
};

class FinanceManager {
private:
    std::map<std::string, Wallet> wallets;
    std::map<std::string, ExpenseCategory> expenseCategories;
    std::map<std::string, std::map<std::string, double>> expenseRecords; 

public:
    void addWallet(const std::string& walletName) {
        wallets.emplace(walletName, Wallet());
    }

    void depositToWallet(const std::string& walletName, double amount) {
        wallets[walletName].deposit(amount);
    }

    bool withdrawFromWallet(const std::string& walletName, double amount) {
        return wallets[walletName].withdraw(amount);
    }

    void addExpenseCategory(const std::string& categoryName) {
        expenseCategories.emplace(categoryName, ExpenseCategory{ categoryName, 0.0 });
    }

    void recordExpense(const std::string& categoryName, double amount) {
        std::time_t now = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&now);
        char buffer[80];

        std::strftime(buffer, 80, "%Y-%m-%d", timeinfo);
        std::string currentDate = buffer;

        std::strftime(buffer, 80, "%Y-%W", timeinfo);
        std::string currentWeek = buffer;

        std::strftime(buffer, 80, "%Y-%m", timeinfo);
        std::string currentMonth = buffer;

        expenseCategories[categoryName].totalAmount += amount;
        expenseRecords[currentDate][categoryName] += amount;
        expenseRecords[currentWeek][categoryName] += amount;
        expenseRecords[currentMonth][categoryName] += amount;
    }

    void generateReport(const std::string& period) {
        std::time_t now = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&now);
        char buffer[80];

        std::string periodKey;
        if (period == "day") {
            std::strftime(buffer, 80, "%Y-%m-%d", timeinfo);
            periodKey = buffer;
        }
        else if (period == "week") {
            std::strftime(buffer, 80, "%Y-%W", timeinfo);
            periodKey = buffer;
        }
        else if (period == "month") {
            std::strftime(buffer, 80, "%Y-%m", timeinfo);
            periodKey = buffer;
        }
        else {
            std::cerr << "Invalid period specified" << std::endl;
            return;
        }

        std::cout << "Report for " << period << ":" << std::endl;
        for (const auto& record : expenseRecords[periodKey]) {
            std::cout << record.first << ": " << record.second << std::endl;
        }
    }

    void saveReportsToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& record : expenseRecords) {
                file << "Period: " << record.first << std::endl;
                for (const auto& category : record.second) {
                    file << category.first << ": " << category.second << std::endl;
                }
            }
            file.close();
            std::cout << "Reports saved to file: " << filename << std::endl;
        }
        else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }
};

int main() {
    FinanceManager financeManager;

    financeManager.addWallet("Wallet1");
    financeManager.addExpenseCategory("Food");

    financeManager.depositToWallet("Wallet1", 1000.0);
    financeManager.recordExpense("Food", 50.0);
    financeManager.recordExpense("Food", 30.0);

    financeManager.generateReport("day");
    financeManager.generateReport("week");
    financeManager.generateReport("month");

    financeManager.saveReportsToFile("reports.txt");

    return 0;
}
