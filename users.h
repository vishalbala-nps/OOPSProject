#include <map>
#ifndef USERS_H
#define USERS_H
#include "medicine.h"
using namespace std;

enum ROLES { ADMIN, DOCTOR, CUSTOMER };

class User {
protected:
  static int lastAdd;
  int userId;
  string username;
  string password;

public:
  User(string, string);
  virtual ~User() {}
  int getUserId();
  string getUsername();
  bool verifyPassword(string);
  void setPassword(string password);
  virtual ROLES getRole() = 0;
  virtual void displayDetails() = 0;
};

class Admin : public User {
public:
  Admin(string username, string password) : User(username, password) {}
  ROLES getRole() override;
  void displayDetails() override;
};

class Doctor : public User {
public:
  Doctor(string username, string password) : User(username, password) {}
  ROLES getRole() override;
  void displayDetails() override;
};

class Customer : public User {
private:
  // vector<Medicine*> cart;
  map<int, Medicine *> cart;

public:
  Customer(string username, string password) : User(username, password) {}
  ROLES getRole() override;
  void addToCart(Medicine *medicine);
  void removeFromCart(int index);
  map<int, Medicine *> getCart();
  void clearCart();
  void displayDetails() override;
};

#endif