#include "users.h"
#include "medicine.h"
#include <iostream>
using namespace std;

User::User(string username, string password) {
  this->username = username;
  this->password = password;
  this->userId = ++lastAdd;
}

int User::getUserId() { return userId; }

string User::getUsername() { return username; }

bool User::verifyPassword(string inpass) { return inpass == password; }

void User::setPassword(string password) { this->password = password; }

ROLES Admin::getRole() { return ADMIN; }

void Admin::displayDetails() {
  cout << "  ----------------------------" << endl;
  cout << "  Role     : Admin" << endl;
  cout << "  User ID  : " << userId << endl;
  cout << "  Username : " << username << endl;
  cout << "  ----------------------------" << endl;
}

ROLES Doctor::getRole() { return DOCTOR; }

void Doctor::displayDetails() {
  cout << "  ----------------------------" << endl;
  cout << "  Role     : Doctor" << endl;
  cout << "  User ID  : " << userId << endl;
  cout << "  Username : " << username << endl;
  cout << "  ----------------------------" << endl;
}

ROLES Customer::getRole() { return CUSTOMER; }

// Bug 1 Fix: accumulate quantity if same medicine added again
void Customer::addToCart(Medicine *medicine, int qty) {
  if (cart.count(medicine->getId())) {
    cart[medicine->getId()].second += qty;
  } else {
    cart[medicine->getId()] = {medicine, qty};
  }
}

void Customer::removeFromCart(int id) { cart.erase(id); }

map<int, pair<Medicine *, int>> Customer::getCart() { return cart; }

void Customer::clearCart() { cart.clear(); }

void Customer::displayDetails() {
  cout << "  ----------------------------" << endl;
  cout << "  Role     : Customer" << endl;
  cout << "  User ID  : " << userId << endl;
  cout << "  Username : " << username << endl;
  cout << "  ----------------------------" << endl;
}
