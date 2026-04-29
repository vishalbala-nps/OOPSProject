#include <iostream>
#include "users.h"
#include "medicine.h"
#include "orders.h"
#include "errors.h"
using namespace std;

int Medicine::lastAdd = 0;
int Orders::lastAdd = 0;
int User::lastAdd = 0;


void viewCustomers(const vector<User*>& users) {
    cout << endl << "Customers:" << endl;
    for (int i=0; i<users.size(); i++) {
        if (users[i]->getRole() == CUSTOMER) {
            users[i]->displayDetails();
            cout << endl;
        }
    }
}

void viewAvailableMedicines(const unordered_map<int,Medicine*> &meds, unordered_map<int,int> &inventory) {
    cout << endl << "Medicines:" << endl;
    for (auto it = meds.begin(); it != meds.end(); ++it) {
        if (inventory[it->first] > 0) {
            it->second->displayDetails();
            cout << "    Available Qty : " << inventory[it->first] << endl;
            cout << endl;
        }
    }
}

void createAndAddUser(vector<User*>& users, ROLES roleType) {
    string n, p;
    string roleName;
    if (roleType == ADMIN) roleName = "Admin";
    else if (roleType == DOCTOR) roleName = "Doctor";
    else roleName = "Customer";

    if (roleType == CUSTOMER) {
        cout << "Enter username: ";
    } else {
        cout << "Enter username of " << roleName << ": ";
    }
    cin >> n;

    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == n) {
            cout << "Error: Username '" << n << "' already exists! Account creation failed." << endl;
            return;
        }
    }

    cout << "Enter password: ";
    cin >> p;

    if (roleType == ADMIN) {
        users.push_back(new Admin(n, p));
    } else if (roleType == DOCTOR) {
        users.push_back(new Doctor(n, p));
    } else {
        users.push_back(new Customer(n, p));
    }

    if (roleType == CUSTOMER) {
        cout << "Account created successfully!" << endl;
    } else {
        cout << roleName << " added successfully!" << endl;
    }
    cout << "ID: " << users.back()->getUserId() << endl;
}

void processAdmin(Admin* u,vector<User*> &users,unordered_map<int,vector<Orders>> &orders,unordered_map<int,Medicine*> &meds,unordered_map<int,int> &inventory) {
    int op;
    while (true) {
        cout << endl << "=====================================" << endl;
        cout << "             ADMIN MENU              " << endl;
        cout << "=====================================" << endl;
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
        cout << "-------------------------------------" << endl;
        cout << "Enter option: ";
        cin >> op;
        try {
            if (op == 1) {
                Medicine *newmed;
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
                        newmed = new OTCMedicine(n, p);
                        break;
                    } else if (mt == 2) {
                        newmed = new PrescriptionMedicine(n, p);
                        break;
                    } else {
                        cout << "Invalid option!" << endl;
                    }
                }
                cout << "Enter quantity: ";
                cin >> qty;
                if (qty < 0) {
                    delete newmed;
                    throw InvalidNoException();
                }
                meds[newmed->getId()] = newmed;
                inventory[newmed->getId()] = qty;
                cout << "Medicine added successfully!" << endl;
                cout << "ID: " << newmed->getId() << endl;
            } else if (op == 2) {
                cout << endl << "Medicines:" << endl;
                for (auto it = meds.begin(); it != meds.end(); ++it) {
                    it->second->displayDetails();
                    cout << "    Available Qty : " << inventory[it->first] << endl;
                    cout << endl;
                }
            } else if (op == 3) {
                int id,qty;
                cout << "Enter medicine ID: ";
                cin >> id;
                cout << "Enter quantity: ";
                cin >> qty;
                if (qty < 0) throw InvalidNoException();
                if (meds.find(id) == meds.end()) {
                    cout << "Medicine ID not found!" << endl;
                } else {
                    inventory[id] += qty;
                    cout << "Inventory updated successfully!" << endl;
                }
            } else if (op == 4) {
                for (auto it = orders.begin(); it != orders.end(); ++it) {
                    for (int i=0;i<it->second.size();i++) {
                        cout << "Customer ID: " << it->first << endl;
                        it->second[i].displayDetails();
                        cout << endl;
                    }
                }
            } else if (op == 5) {
                createAndAddUser(users, DOCTOR);
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
                createAndAddUser(users, ADMIN);
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

void processDoctor(Doctor* u, vector<User*>& users, unordered_map<int,Medicine*> &meds, unordered_map<int,int> &inventory, unordered_map<int,vector<int>> &prescriptions) {
    int op;
    while (true) {
        cout << endl << "=====================================" << endl;
        cout << "             DOCTOR MENU             " << endl;
        cout << "=====================================" << endl;
        cout << "1. Prescribe medicines" << endl;
        cout << "2. View medicines" << endl;
        cout << "3. View account details" << endl;
        cout << "4. View customers" << endl;
        cout << "5. Logout" << endl;
        cout << "-------------------------------------" << endl;
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
            bool medExists = (meds.find(medId) != meds.end());
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

void processCustomer(Customer* u, unordered_map<int,Medicine*> &meds, unordered_map<int,int> &inventory, unordered_map<int,vector<int>> &prescriptions, unordered_map<int,vector<Orders>> &orders) {
    int op;
    while (true) {
        cout << endl << "=====================================" << endl;
        cout << "            CUSTOMER MENU            " << endl;
        cout << "=====================================" << endl;
        cout << "1. View all medicines" << endl;
        cout << "2. Add medicine to cart" << endl;
        cout << "3. View cart" << endl;
        cout << "4. Place order" << endl;
        cout << "5. View account details" << endl;
        cout << "6. Logout" << endl;
        cout << "-------------------------------------" << endl;
        cout << "Enter option: ";
        cin >> op;
        try {
            if (op == 1) {
                viewAvailableMedicines(meds, inventory);
            } else if (op == 2) {
                int medId;
                cout << "Enter Medicine ID: ";
                cin >> medId;
                if (meds.find(medId) == meds.end()) {
                    throw MedicineNotFoundException();
                }
                if (inventory[medId] <= 0) {
                    throw OutOfStockException();
                }
                u->addToCart(meds[medId]);
                cout << "Medicine added to cart!" << endl;
            } else if (op == 3) {
                vector<Medicine*> cart = u->getCart();
                cout << endl << "--- Your Cart ---" << endl;
                if (cart.empty()) {
                    cout << "Your cart is currently empty." << endl;
                } else {
                    double total = 0;
                    for (int i = 0; i < cart.size(); i++) {
                        cart[i]->displayDetails();
                        total += cart[i]->getPrice();
                    }
                    cout << endl << "Total Price: Rs." << total << endl;
                }
                cout << "-----------------" << endl;
            } else if (op == 4) {
                vector<Medicine*> cart = u->getCart();
                if (cart.empty()) {
                    cout << "Cart is empty! Please add items before placing an order." << endl;
                    continue;
                }
                string deliveryLocation;
                cout << "Enter delivery location: ";
                cin >> deliveryLocation;
                
                int userId = u->getUserId();
                for (int i = 0; i < cart.size(); i++) {
                    Medicine* currentMed = cart[i];
                    int medId = currentMed->getId();
                    
                    if (dynamic_cast<PrescriptionMedicine*>(currentMed) != nullptr) {
                        bool hasPrescription = false;
                        if (prescriptions.find(userId) != prescriptions.end()) {
                            vector<int>& userPrescriptions = prescriptions[userId];
                            for (int j = 0; j < userPrescriptions.size(); j++) {
                                if (userPrescriptions[j] == medId) {
                                    hasPrescription = true;
                                    break;
                                }
                            }
                        }
                        if (!hasPrescription) {
                            cout << "Order failed: Medicine " << currentMed->getMedicine() << " (" << currentMed->getId() << ") requires a prescription!" << endl;
                            continue;
                        }
                    }
                    
                    if (inventory[medId] > 0) {
                        int numInputQuantity = 1; // Assuming 1 per item added
                        orders[userId].push_back(Orders(medId, numInputQuantity, deliveryLocation));
                        inventory[medId] -= numInputQuantity;
                        cout << "Order placed successfully for Medicine: " << meds[medId]->getMedicine() << endl;
                    } else {
                        cout << "Medicine " << meds[medId]->getMedicine() << " is out of stock!" << endl;
                    }
                }
                u->clearCart();
            } else if (op == 5) {
                cout << endl << "Account Details:" << endl;
                u->displayDetails();
                cout << endl;
            } else if (op == 6) {
                break;
            } else {
                cout << "Invalid option!" << endl;
            }
        } catch (const MedicineNotFoundException &e) {
            cout << "Error: " << e.what() << " Please check the ID and try again." << endl;
        } catch (const OutOfStockException &e) {
            cout << "Error: " << e.what() << " Please wait for restock." << endl;
        } catch (const exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

int main() {
    vector<User*> users;
    unordered_map<int,vector<Orders>> orders;
    unordered_map<int,Medicine*> medicines;
    unordered_map<int,int> inventory;
    unordered_map<int,vector<int>> prescriptions;
    users.push_back(new Admin("admin", "admin"));
    while (true) {
        cout << endl << "=====================================" << endl;
        cout << "      PHARMACY MANAGEMENT SYSTEM     " << endl;
        cout << "=====================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. New Customer" << endl;
        cout << "3. Exit" << endl;
        cout << "-------------------------------------" << endl;
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
                        processCustomer(dynamic_cast<Customer*>(users[i]), medicines, inventory, prescriptions, orders);
                    }
                }
            }
            if (!loginSuccess) {
                cout << "Invalid username or password!" << endl;
            }
        } else if (choice == 2) {
            createAndAddUser(users, CUSTOMER);
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid option!" << endl;
        }
    }
}