#include <iostream>
#include "users.h"
#include "medicine.h"
#include "orders.h"
#include "errors.h"
using namespace std;

void viewCustomers(const vector<User*>& users) {
    cout << endl << "Customers:" << endl;
    for (int i=0; i<users.size(); i++) {
        if (users[i]->getRole() == CUSTOMER) {
            users[i]->displayDetails();
            cout << endl;
        }
    }
}

void processAdmin(Admin* u,vector<User*> &users,vector<Orders> &orders,vector<Medicine*> &meds,unordered_map<int,int> &inventory) {
    int op;
    while (true) {
        cout << endl << "Admin Menu:" << endl;
        cout << "1. Add Medicine" << endl;
        cout << "2. View Medicines" << endl;
        cout << "3. Update Inventory" << endl;
        cout << "4. View Orders" << endl;
        cout << "5. Add Doctor" << endl;
        cout << "6. View Doctors" << endl;
        cout << "7. View Customers" << endl;
        cout << "8. Add Admin" << endl;
        cout << "9. Get Account details" << endl;
        cout << "10. Logout" << endl;
        cout << "Enter option: ";
        cin >> op;
        try {
            if (op == 1) {
                string n;
                double p;
                int qty;
                cout << "Enter name of Medicine: ";
                cin >> n;
                cout << "Enter price: ";
                cin >> p;
                if (p < 0) throw InvalidNoException();
                while (true) {
                    int mt;
                    cout << "Enter medicine type" << endl;
                    cout << "1. Over the Counter" << endl;
                    cout << "2. Prescription" << endl;
                    cout << "Enter option: ";
                    cin >> mt;
                    if (mt == 1) {
                        meds.push_back(new OTCMedicine(n, p));
                        break;
                    } else if (mt == 2) {
                        meds.push_back(new PrescriptionMedicine(n, p));
                        break;
                    } else {
                        cout << "Invalid option!" << endl;
                    }
                }
                cout << "Enter quantity: ";
                cin >> qty;
                if (qty < 0) throw InvalidNoException();
                inventory[meds.back()->getId()] = qty;
                cout << "Medicine added successfully!" << endl;
                cout << "ID: " << meds.back()->getId() << endl;
            } else if (op == 2) {
                cout << endl << "Medicines:" << endl;
                for (int i = 0; i < meds.size(); i++) {
                    meds[i]->displayDetails();
                    cout << "Available Quantity: " << inventory[meds[i]->getId()] << endl;
                    cout << endl;
                }
            } else if (op == 3) {
                int id,qty;
                cout << "Enter medicine ID: ";
                cin >> id;
                cout << "Enter quantity: ";
                cin >> qty;
                if (qty < 0) throw InvalidNoException();
                inventory[id] += qty;
                cout << "Inventory updated successfully!" << endl;
            } else if (op == 4) {
                for (int i=0;i<orders.size();i++) {
                    orders[i].displayDetails();
                    cout << endl;
                }
            } else if (op == 5) {
                string n,p;
                cout << "Enter username of Doctor: ";
                cin >> n;
                cout << "Enter password: ";
                cin >> p;
                users.push_back(new Doctor(n,p));
                cout << "Doctor added successfully!" << endl;
                cout << "ID: " << users.back()->getUserId() << endl;
            } else if (op == 6) {
                cout << endl << "Doctors:" << endl;
                for (int i=0;i<users.size();i++) {
                    if (users[i]->getRole() == DOCTOR) {
                        users[i]->displayDetails();
                        cout << endl;
                    }
                }
            } else if (op == 7) {
                viewCustomers(users);
            } else if (op == 8) {
                string n,p;
                cout << "Enter username of Admin: ";
                cin >> n;
                cout << "Enter password: ";
                cin >> p;
                users.push_back(new Admin(n,p));
                cout << "Admin added successfully!" << endl;
                cout << "ID: " << users.back()->getUserId() << endl;
            } else if (op == 9) {
                cout << endl << "Account Details:" << endl;
                u->displayDetails();
                cout << endl;
            } else if (op == 10) {
                break;
            } else {
                cout << "Invalid option!" << endl;
            }
        } catch (InvalidNoException &e) {
            cout << "Value cannot be negative!" << endl;
        }
    }
}

void viewAvailableMedicines(const vector<Medicine*> &meds, unordered_map<int,int> &inventory) {
    cout << endl << "Medicines:" << endl;
    for (int i = 0; i < meds.size(); i++) {
        if (inventory[meds[i]->getId()] > 1) {
            meds[i]->displayDetails();
            cout << "Available Quantity: " << inventory[meds[i]->getId()] << endl;
            cout << endl;
        }
    }
}

void processDoctor(Doctor* u, vector<User*>& users, vector<Medicine*> &meds, unordered_map<int,int> &inventory, unordered_map<int,vector<int>> &prescriptions) {
    int op;
    while (true) {
        cout << endl << "Doctor Menu:" << endl;
        cout << "1. Prescribe medicines" << endl;
        cout << "2. View medicines" << endl;
        cout << "3. View account details" << endl;
        cout << "4. View customers" << endl;
        cout << "5. Logout" << endl;
        cout << "Enter option: ";
        cin >> op;
        if (op == 1) {
            int medId, userId;
            cout << "Enter Medicine ID: ";
            cin >> medId;
            cout << "Enter User ID: ";
            cin >> userId;
            
            bool userExists = false;
            for (int i = 0; i < users.size(); i++) {
                if (users[i]->getUserId() == userId && users[i]->getRole() == CUSTOMER) {
                    userExists = true;
                    break;
                }
            }
            bool medExists = false;
            for (int i = 0; i < meds.size(); i++) {
                if (meds[i]->getId() == medId) {
                    medExists = true;
                    break;
                }
            }
            if (userExists && medExists) {
                prescriptions[userId].push_back(medId);
                cout << "Medicine prescribed successfully!" << endl;
            } else {
                cout << "Invalid User ID or Medicine ID!" << endl;
            }
        } else if (op == 2) {
            viewAvailableMedicines(meds, inventory);
        } else if (op == 3) {
            cout << endl << "Account Details:" << endl;
            u->displayDetails();
            cout << endl;
        } else if (op == 4) {
            viewCustomers(users);
        } else if (op == 5) {
            break;
        } else {
            cout << "Invalid option!" << endl;
        }
    }
}

int main() {
    vector<User*> users;
    vector<Orders> orders;
    vector<Medicine*> medicines;
    unordered_map<int,int> inventory;
    unordered_map<int,vector<int>> prescriptions;
    users.push_back(new Admin("admin", "admin"));
    while (true) {
        cout << endl << "Menu:" << endl;
        cout << "1. Login" << endl;
        cout << "2. New Customer" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter option: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            string username;
            string password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            bool loginSuccess = false;
            for (int i=0;i<users.size();i++) {
                if (users[i]->getUsername() == username && users[i]->verifyPassword(password)) {
                    loginSuccess = true;
                    if (users[i]->getRole() == ADMIN) {
                        processAdmin(dynamic_cast<Admin*>(users[i]),users,orders,medicines,inventory);
                    } else if (users[i]->getRole() == DOCTOR) {
                        processDoctor(dynamic_cast<Doctor*>(users[i]),users,medicines,inventory,prescriptions);
                    } else if (users[i]->getRole() == CUSTOMER) {
                        
                    }
                }
            }
            if (!loginSuccess) {
                cout << "Invalid username or password!" << endl;
            }
        } else if (choice == 2) {
            string n,p;
            cout << "Enter username: ";
            cin >> n;
            cout << "Enter password: ";
            cin >> p;
            users.push_back(new Customer(n,p));
            cout << "Account created successfully!" << endl;
            cout << "ID: " << users.back()->getUserId() << endl;
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid option!" << endl;
        }
    }
}