#include "errors.h"
#include "medicine.h"
#include "orders.h"
#include "users.h"
#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;

int Medicine::lastAdd = 0;
int Orders::lastAdd = 0;
int User::lastAdd = 0;

void viewCustomers(const vector<User *> &users) {
  cout << endl << "Customers:" << endl;
  for (int i = 0; i < users.size(); i++) {
    if (users[i]->getRole() == CUSTOMER) {
      users[i]->displayDetails();
      cout << endl;
    }
  }
}

void viewAvailableMedicines(const unordered_map<int, Medicine *> &meds,
                            unordered_map<int, int> &inventory) {
  cout << endl << "Medicines:" << endl;
  for (auto it = meds.begin(); it != meds.end(); ++it) {
    // Bug 7 Fix: use count+at instead of [] to avoid silent zero-insertion
    if (inventory.count(it->first) && inventory.at(it->first) > 0) {
      it->second->displayDetails();
      cout << "    Available Qty : " << inventory.at(it->first) << endl;
      cout << endl;
    }
  }
}

void createAndAddUser(vector<User *> &users, ROLES roleType) {
  string n, p;
  string roleName;
  if (roleType == ADMIN)
    roleName = "Admin";
  else if (roleType == DOCTOR)
    roleName = "Doctor";
  else
    roleName = "Customer";

  if (roleType == CUSTOMER) {
    cout << "Enter username: ";
  } else {
    cout << "Enter username of " << roleName << ": ";
  }
  cin >> n;

  for (int i = 0; i < users.size(); i++) {
    if (users[i]->getUsername() == n) {
      cout << "Error: Username '" << n
           << "' already exists! Account creation failed." << endl;
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

void processAdmin(Admin *u, vector<User *> &users,
                  unordered_map<int, vector<Orders>> &orders,
                  unordered_map<int, Medicine *> &meds,
                  unordered_map<int, int> &inventory) {
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
    cout << "10. Update order status" << endl;
    cout << "11. Logout" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Enter option: ";
    cin >> op;
    try {
      if (op == 1) {
        string n;
        double p;
        int qty;
        int mt;

        // Bug 5 Fix: use getline so medicine names with spaces work
        cout << "Enter name of Medicine: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, n);

        // Bug 10 Fix: check name uniqueness before creating
        bool nameExists = false;
        for (auto it = meds.begin(); it != meds.end(); ++it) {
          if (it->second->getMedicine() == n) {
            nameExists = true;
            break;
          }
        }
        if (nameExists) {
          cout << "Error: Medicine with this name already exists!" << endl;
          continue;
        }

        cout << "Enter price: ";
        cin >> p;
        if (p < 0)
          throw InvalidNoException();

        while (true) {
          cout << "Enter medicine type" << endl;
          cout << "1. Over the Counter" << endl;
          cout << "2. Prescription" << endl;
          cout << "Enter option: ";
          cin >> mt;
          if (mt == 1 || mt == 2) break;
          cout << "Invalid option!" << endl;
        }

        // Bug 8 Fix: validate qty BEFORE constructing medicine object
        // so lastAdd is not incremented on a failed creation
        cout << "Enter quantity: ";
        cin >> qty;
        if (qty < 0)
          throw InvalidNoException();

        Medicine *newmed;
        if (mt == 1) {
          newmed = new OTCMedicine(n, p);
        } else {
          newmed = new PrescriptionMedicine(n, p);
        }
        meds[newmed->getId()] = newmed;
        inventory[newmed->getId()] = qty;
        cout << "Medicine added successfully!" << endl;
        cout << "ID: " << newmed->getId() << endl;

      } else if (op == 2) {
        cout << endl << "Medicines:" << endl;
        for (auto it = meds.begin(); it != meds.end(); ++it) {
          it->second->displayDetails();
          // Bug 7 Fix: use count+at instead of []
          int qty = inventory.count(it->first) ? inventory.at(it->first) : 0;
          cout << "    Available Qty : " << qty << endl;
          cout << endl;
        }
      } else if (op == 3) {
        int id, qty;
        cout << "Enter medicine ID: ";
        cin >> id;
        cout << "Enter quantity: ";
        cin >> qty;
        if (qty < 0)
          throw InvalidNoException();
        if (meds.find(id) == meds.end()) {
          cout << "Medicine ID not found!" << endl;
        } else {
          inventory[id] += qty;
          cout << "Inventory updated successfully!" << endl;
        }
      } else if (op == 4) {
        for (auto it = orders.begin(); it != orders.end(); ++it) {
          for (int i = 0; i < it->second.size(); i++) {
            cout << "Customer ID: " << it->first << endl;
            it->second[i].displayDetails();
            cout << endl;
          }
        }
      } else if (op == 5) {
        createAndAddUser(users, DOCTOR);
      } else if (op == 6) {
        cout << endl << "Doctors:" << endl;
        for (int i = 0; i < users.size(); i++) {
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
        int oid;
        cout << "Enter Order ID: ";
        cin >> oid;
        cout << "Enter new status (1. Processing, 2. Shipped, 3. Delivered): ";
        int st_choice;
        cin >> st_choice;
        OrderStatus st;
        if (st_choice == 1)
          st = PROCESSING;
        else if (st_choice == 2)
          st = SHIPPED;
        else if (st_choice == 3)
          st = DELIVERED;
        else {
          cout << "Invalid status!" << endl;
          continue;
        }

        bool found = false;
        for (auto it = orders.begin(); it != orders.end(); ++it) {
          for (int i = 0; i < it->second.size(); i++) {
            if (it->second[i].getOrderId() == oid) {
              // Bug 9 Fix: prevent moving status backwards
              if (st <= it->second[i].getStatus()) {
                cout << "Error: Cannot move order status backwards!" << endl;
              } else {
                it->second[i].setStatus(st);
                cout << "Order status updated successfully!" << endl;
              }
              found = true;
              break;
            }
          }
          if (found)
            break;
        }
        if (!found)
          cout << "Order not found!" << endl;
      } else if (op == 11) {
        break;
      } else {
        cout << "Invalid option!" << endl;
      }
    } catch (InvalidNoException &e) {
      cout << "Value cannot be negative!" << endl;
    }
  }
}

void processDoctor(Doctor *u, vector<User *> &users,
                   unordered_map<int, Medicine *> &meds,
                   unordered_map<int, int> &inventory,
                   unordered_map<int, vector<int>> &prescriptions) {
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
        if (users[i]->getUserId() == userId &&
            users[i]->getRole() == CUSTOMER) {
          userExists = true;
          break;
        }
      }
      bool medExists = (meds.find(medId) != meds.end());

      if (userExists && medExists) {
        // Bug 4 Fix: only allow prescribing PrescriptionMedicine
        if (dynamic_cast<PrescriptionMedicine *>(meds[medId]) == nullptr) {
          cout << "Error: This is an OTC medicine and does not require a prescription!" << endl;
        } else {
          // Bug 11 Fix: prevent duplicate prescriptions for same patient
          bool alreadyPrescribed = false;
          if (prescriptions.count(userId)) {
            for (int id : prescriptions[userId]) {
              if (id == medId) {
                alreadyPrescribed = true;
                break;
              }
            }
          }
          if (alreadyPrescribed) {
            cout << "Error: This medicine is already prescribed to this patient!" << endl;
          } else {
            prescriptions[userId].push_back(medId);
            cout << "Medicine prescribed successfully!" << endl;
          }
        }
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

void processCustomer(Customer *u, unordered_map<int, Medicine *> &meds,
                     unordered_map<int, int> &inventory,
                     unordered_map<int, vector<int>> &prescriptions,
                     unordered_map<int, vector<Orders>> &orders) {
  int op;
  while (true) {
    cout << endl << "=====================================" << endl;
    cout << "            CUSTOMER MENU            " << endl;
    cout << "=====================================" << endl;
    cout << "1. View all medicines" << endl;
    cout << "2. Add medicine to cart" << endl;
    cout << "3. View cart" << endl;
    cout << "4. Remove item from cart" << endl;
    cout << "5. Place order" << endl;
    cout << "6. View my orders" << endl;
    cout << "7. View account details" << endl;
    cout << "8. Logout" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Enter option: ";
    cin >> op;
    try {
      if (op == 1) {
        viewAvailableMedicines(meds, inventory);
      } else if (op == 2) {
        int medId, qty;
        cout << "Enter Medicine ID: ";
        cin >> medId;
        if (meds.find(medId) == meds.end()) {
          throw MedicineNotFoundException();
        }
        // Bug 12 Fix: use count+at to avoid silent zero-insertion
        if (!inventory.count(medId) || inventory.at(medId) <= 0) {
          throw OutOfStockException();
        }
        // Bug 1 & 2 Fix: ask for quantity
        cout << "Enter quantity: ";
        cin >> qty;
        if (qty <= 0)
          throw InvalidNoException();
        // Workflow Bug A Fix: subtract what's already in cart before checking stock
        map<int, pair<Medicine *, int>> currentCart = u->getCart();
        int alreadyInCart = currentCart.count(medId) ? currentCart.at(medId).second : 0;
        int effectiveAvailable = inventory.at(medId) - alreadyInCart;
        if (effectiveAvailable <= 0) {
          cout << "Error: All available stock is already in your cart!" << endl;
          continue;
        }
        if (effectiveAvailable < qty) {
          cout << "Error: Only " << effectiveAvailable << " more unit(s) can be added (rest already in cart)!" << endl;
          continue;
        }
        u->addToCart(meds[medId], qty);
        cout << "Medicine added to cart!" << endl;
      } else if (op == 3) {
        map<int, pair<Medicine *, int>> cart = u->getCart();
        if (cart.empty()) {
          cout << "Your cart is empty!" << endl;
          continue;
        }
        // Bug 3 Fix: multiply price by quantity for correct total
        double total = 0;
        for (auto it = cart.begin(); it != cart.end(); ++it) {
          it->second.first->displayDetails();
          cout << "    Quantity    : " << it->second.second << endl;
          total += it->second.first->getPrice() * it->second.second;
        }
        cout << endl << "Total Price: Rs." << total << endl;
        cout << "-----------------" << endl;
      } else if (op == 4) {
        // Workflow Bug B Fix: allow removing a specific item from cart
        map<int, pair<Medicine *, int>> cart = u->getCart();
        if (cart.empty()) {
          cout << "Cart is empty!" << endl;
          continue;
        }
        int removeId;
        cout << "Enter Medicine ID to remove from cart: ";
        cin >> removeId;
        if (!cart.count(removeId)) {
          cout << "Medicine ID " << removeId << " is not in your cart!" << endl;
        } else {
          u->removeFromCart(removeId);
          cout << "Item removed from cart." << endl;
        }
      } else if (op == 5) {
        map<int, pair<Medicine *, int>> cart = u->getCart();
        vector<int> toRemove;
        if (cart.empty()) {
          cout << "Cart is empty! Please add items before placing an order."
               << endl;
          continue;
        }
        string deliveryLocation;
        cout << "Enter delivery location: ";
        // Bug 5 Fix: getline for multi-word delivery locations
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, deliveryLocation);

        int userId = u->getUserId();
        for (auto it = cart.begin(); it != cart.end(); ++it) {
          Medicine *currentMed = it->second.first;
          int numInputQuantity = it->second.second; // Bug 2 Fix: use cart qty
          int medId = currentMed->getId();

          if (dynamic_cast<PrescriptionMedicine *>(currentMed) != nullptr) {
            bool hasPrescription = false;
            if (prescriptions.find(userId) != prescriptions.end()) {
              vector<int> &userPrescriptions = prescriptions[userId];
              for (int j = 0; j < userPrescriptions.size(); j++) {
                if (userPrescriptions[j] == medId) {
                  hasPrescription = true;
                  break;
                }
              }
            }
            if (!hasPrescription) {
              cout << "Order failed: Medicine " << currentMed->getMedicine()
                   << " (" << currentMed->getId()
                   << ") requires a prescription!" << endl;
              continue;
            }
          }

          // Bug 12 Fix: use count+at, also check sufficient stock for qty
          if (inventory.count(medId) && inventory.at(medId) >= numInputQuantity) {
            orders[userId].push_back(
                Orders(medId, numInputQuantity, deliveryLocation));
            inventory[medId] -= numInputQuantity;
            cout << "Order placed successfully for Medicine: "
                 << meds[medId]->getMedicine()
                 << " (Qty: " << numInputQuantity << ")" << endl;
            toRemove.push_back(medId);

            if (dynamic_cast<PrescriptionMedicine *>(currentMed) != nullptr) {
              vector<int> &userPrescriptions = prescriptions[userId];
              for (auto itP = userPrescriptions.begin();
                   itP != userPrescriptions.end(); ++itP) {
                if (*itP == medId) {
                  userPrescriptions.erase(itP);
                  break;
                }
              }
            }
          } else {
            cout << "Medicine " << meds[medId]->getMedicine()
                 << " does not have sufficient stock!" << endl;
          }
        }
        for (int i = 0; i < toRemove.size(); i++) {
          u->removeFromCart(toRemove[i]);
        }
      } else if (op == 6) {
        int uid = u->getUserId();
        if (orders.find(uid) == orders.end() || orders[uid].empty()) {
          cout << "You have no orders!" << endl;
        } else {
          cout << endl << "--- Your Orders ---" << endl;
          for (int i = 0; i < orders[uid].size(); i++) {
            orders[uid][i].displayDetails();
            cout << endl;
          }
          cout << "-------------------" << endl;
        }
      } else if (op == 7) {
        cout << endl << "Account Details:" << endl;
        u->displayDetails();
        cout << endl;
      } else if (op == 8) {
        u->clearCart(); // Bug 6 Fix: clear cart on logout
        break;
      } else {
        cout << "Invalid option!" << endl;
      }
    } catch (const MedicineNotFoundException &e) {
      cout << "Error: " << e.what() << " Please check the ID and try again."
           << endl;
    } catch (const OutOfStockException &e) {
      cout << "Error: " << e.what() << " Please wait for restock." << endl;
    } catch (const InvalidNoException &e) {
      cout << "Error: " << e.what() << endl;
    } catch (const exception &e) {
      cout << "Error: " << e.what() << endl;
    }
  }
}

int main() {
  vector<User *> users;
  unordered_map<int, vector<Orders>> orders;
  unordered_map<int, Medicine *> medicines;
  unordered_map<int, int> inventory;
  unordered_map<int, vector<int>> prescriptions;
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
      for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == username &&
            users[i]->verifyPassword(password)) {
          loginSuccess = true;
          if (users[i]->getRole() == ADMIN) {
            processAdmin(dynamic_cast<Admin *>(users[i]), users, orders,
                         medicines, inventory);
          } else if (users[i]->getRole() == DOCTOR) {
            processDoctor(dynamic_cast<Doctor *>(users[i]), users, medicines,
                          inventory, prescriptions);
          } else if (users[i]->getRole() == CUSTOMER) {
            processCustomer(dynamic_cast<Customer *>(users[i]), medicines,
                            inventory, prescriptions, orders);
          }
          break;
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
