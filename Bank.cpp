//Step 1- Setting up the Project
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdio>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;
//Declare Global Variables
int accno,pin;

//Step 2- Designing the Menu System
class Menu
{
    public:
    void welcomemessage()
    {
        cout<<"______________________"<<endl;
        cout<<"*******************"<<endl;
        cout<<"\t\t\t\t Welcome to the Bank \t\t\t\t"<<endl;
        cout<<"*******************"<<endl;
    }
    void mainmenu()
    {
        cout<<"\t 1.Bank"<<endl;
        cout<<"\t 2.ATM"<<endl;
        cout<<"\t 3.Exit"<<endl;
    }

    void submenu()
    {
        cout<<"\t 1.Opening an Account"<<endl;
        cout<<"\t 2.Check"<<endl;
        cout<<"\t 3.Deposit"<<endl;
        cout<<"\t 4.Transfer"<<endl;
        cout<<"\t 5.Exit"<<endl;
        cout<<"Enter your choice:"<<endl;
    }
};
class NewBank
{
   public:
   string accountName;
   double balance;
   int pincode;

   void getData()
   {
    cout<<"Enter the account Name"<<endl;
    cin>>accountName;
    cout<<"Enter the PinNumber"<<endl;
    cin>>pin;
    cout<<"Enter the Pincode"<<endl;
    cin>>pincode;
    balance=0.0;

    int lastAccNo = 1000;
    //Saving LastAccountNumber details
    ifstream inFile("lastAccountNumber.txt",ios::in);
        if (inFile) inFile >> lastAccNo;
        inFile.close();
        accno = lastAccNo + 1;
    
    //Account file
    ofstream file("Account.txt",ios::app);
    if(file.is_open())
    {
        file<<accno<<" "<<accountName<<" "<<pin<<" "<<balance<<endl;
        file.close();
        cout<<"Data is written";
    }
    ofstream outFile("lastAccountNumber.txt",ios::out);
    outFile << accno;
    outFile.close();

    //printing account details
    cout<<"*******************"<<endl;
    cout<<"Account Opened Successfully"<<accno << endl;
    cout<<"*******************"<<endl;
   }
};
class Customer
{
    public:
    void searchData(int account)
    {
        int acc;
        string name;
        int pin;
        double balance;
        bool found = false;
        ifstream file("Account.txt",ios::in);//reads the value

        while (file >> acc >> name >> pin >> balance) {
            if (acc == account) {
                cout << "Account Found!\nName: " << name << "\nBalance: " << balance << endl;
                found = true;
                break;
            }
        }
        file.close();
        if (!found) cout << "Account not found!" << endl;
    }
};

class LoginChecker {
public:
    bool verifyCredentials(int accNo, int enteredPin) {
        ifstream file("Account.txt", ios::in);
        int acc, pin;
        string name;
        double balance;
        while (file >> acc >> name >> pin >> balance) {
            if (acc == accNo && pin == enteredPin) {
                return true;
            }
        }
        return false;
    }
};

class updateBalance : public LoginChecker {
public:
    void depositMoney(int enteredaccount, int enteredpin, double amt) 
    {
        if (!verifyCredentials(enteredaccount, enteredpin)) {  // Directly call verifyCredentials()
            cout << "Invalid credentials!" << endl;
            return;
        }

        if (amt < 500 || ((int)amt % 500 != 0)) {
            cout << "Deposit amount must be a multiple of 500." << endl;
            return;
        }

        ifstream file("Account.txt", ios::in);
        ofstream temp("temp.txt", ios::out);
        int acc, pin;
        string name;
        double balance;
        bool updated = false;

        while (file >> acc >> name >> pin >> balance) {
            if (acc == enteredaccount) {
                balance += amt;
                updated = true;
            }
            temp << acc << " " << name << " " << pin << " " << balance << endl;
        }

        file.close();
        temp.close();

        if (updated) 
        fs::remove("Account.txt");
        fs::rename("temp.txt", "Account.txt");
        cout << "Deposit successful! New Balance: " << balance << endl;
    }

    void transferMoney(int senderAcc, int senderPin, int receiverAcc, double amount) {
        if (!verifyCredentials(senderAcc, senderPin)) {  // Directly call verifyCredentials()
            cout << "Invalid credentials!" << endl;
            return;
        }

        if (amount < 500 || ((int)amount % 500 != 0)) {
            cout << "Transfer amount must be a multiple of 500." << endl;
            return;
        }

        ifstream file("Account.txt", ios::in);
        ofstream temp("temp.txt", ios::out);

        int acc, pin;
        string name;
        double balance;
        bool senderFound = false, receiverFound = false;

        while (file >> acc >> name >> pin >> balance) {
            if (acc == senderAcc && balance >= amount) {
                balance -= amount;
                senderFound = true;
            }
            if (acc == receiverAcc) {
                balance += amount;
                receiverFound = true;
            }
            temp << acc << " " << name << " " << pin << " " << balance << endl;
        }

        file.close();
        temp.close();

        if (senderFound && receiverFound) {
            fs::rename("temp.txt", "Account.txt");
            cout << "Transfer successful!" << endl;
        } else {
            cout << "Transfer failed! Please check details." << endl;
        }
    }
};

class ATM : public LoginChecker {
    public:
        void checkBalance(int account, int pinno) {
            if (!verifyCredentials(account, pinno)) {
                cout << "Invalid PIN!" << endl;
                return;
            }
            ifstream file("Account.txt",ios::in);
            int acc, p;
            string name;
            double balance;
            while (file >> acc >> name >> p >> balance) {
                if (acc == account) {
                    cout << "Current Balance: " << balance << endl;
                    break;
                }
            }
        }
    
        void withdrawMoney(int account, int pinno, double amt) {
            if (!verifyCredentials(account, pinno)) {
                cout << "Invalid PIN!" << endl;
                return;
            }
            if (amt < 500 || ((int)amt % 500 != 0)) {
                cout << "Withdrawal amount must be in multiples of 500." << endl;
                return;
            }
            ifstream file("Account.txt",ios::in);
            ofstream temp("temp.txt",ios::out);
            int acc, p;
            string name;
            double balance;
            bool updated = false;
    
            while (file >> acc >> name >> p >> balance) {
                if (acc == account && balance >= amt) {
                    balance -= amt;
                    updated = true;
                }
                temp << acc << " " << name << " " << p << " " << balance << endl;
            }
            file.close(); temp.close();
            if (updated) fs::rename("temp.txt", "Account.txt");
            cout << "Withdrawal successful! New Balance: " << balance << endl;
        }
    };

int main() {
    Menu menu;
    NewBank bank;
    Customer customer;
    updateBalance updater;
    LoginChecker checker;
    ATM atm;
    int mchoice,schoice,achoice, accNo, pin, receiverAcc;
    double amount;

    while (true) {
        menu.mainmenu();
        cin >> mchoice;
        
        
        if (mchoice == 1) {
            do {
            menu.submenu();
            cin >> schoice; 

            switch (schoice) {
                case 1:
                    bank.getData();
                    break;
                case 2:
                    cout << "Enter Account Number: ";
                    cin >> accNo;
                    customer.searchData(accNo);
                    break;
                case 3:
                    cout << "Enter Account Number: ";
                    cin >> accNo;
                    cout << "Enter PIN: ";
                    cin >> pin;
                    cout << "Enter amount to deposit: ";
                    cin >> amount;
                    updater.depositMoney(accNo, pin, amount);
                    break;
                case 4:
                    cout << "Enter Sender Account Number: ";
                    cin >> accNo;
                    cout << "Enter PIN: ";
                    cin >> pin;
                    cout << "Enter Receiver Account Number: ";
                    cin >> receiverAcc;
                    cout << "Enter Transfer Amount: ";
                    cin >> amount;
                    updater.transferMoney(accNo, pin, receiverAcc, amount);
                    break;
                case 5:
                    break;
                default: cout << "Invalid choice. Try again." << endl;
            }
        } while (schoice != 5);
        }
        
            else if(mchoice == 2)
            {
            cout << "Enter Account Number & PIN: "; 
            cin >> accNo >> pin;
            cout << "1. Check Balance\n2. Withdraw Money\nEnter choice: "; 
            cin >> achoice;
            if (achoice == 1) 
            atm.checkBalance(accNo, pin);
            else if (achoice == 2) { 
                cout << "Enter amount: "; 
                cin >> amount; 
                atm.withdrawMoney(accNo, pin, amount); 
            }
            }
         else if (mchoice == 3) {
            break;
        }  else {
            cout << "Invalid choice. Try again." << endl;
        }
    }
}




