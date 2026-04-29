#ifndef USERS_H
#define USERS_H
#include <iostream>
#include "medicine.h"
using namespace std;

enum ROLES {ADMIN,DOCTOR,CUSTOMER};

class User {
  protected:
    static int lastAdd;
    int userId;
    string username;
    string password;

  public:
    User(string username, string password) {
      this->username = username;
      this->password = password;
      this->userId = ++lastAdd;
    }
    int getUserId() { 
      return userId; 
    }
    string getUsername() { 
      return username; 
    }
    bool verifyPassword(string inpass) {
      return inpass == password;
    }
    void setPassword(string password) { 
      this->password = password;
    }
    virtual ROLES getRole() = 0;
    virtual void displayDetails() = 0;
};

class Admin : public User {
  public:
    Admin(string username, string password) : User(username, password) {}
    ROLES getRole() override {
      return ADMIN;
    }
    void displayDetails() override {
      cout << "User ID: " << userId << endl;
      cout << "Username: " << username << endl;
      cout << "Role: Admin" << endl;
    }
};

class Doctor : public User {
  public:
    Doctor(string username,string password) : User(username,password) {}
    ROLES getRole() override {
      return DOCTOR;
    }
    void displayDetails() override {
      cout << "User ID: " << userId << endl;
      cout << "Username: " << username << endl;
      cout << "Role: Doctor" << endl;
    }
};

class Customer : public User {
  private:
    vector<Medicine*> cart;
  public:
    Customer(string username,string password) : User(username,password) {}
    ROLES getRole() override {
      return CUSTOMER;
    }
    void addToCart(Medicine* medicine) {
      cart.push_back(medicine);
    }
    vector<Medicine*> getCart() {
      return cart;
    }
    void displayDetails() override {
      cout << "User ID: " << userId << endl;
      cout << "Username: " << username << endl;
      cout << "Role: Customer" << endl;
    }
};

int User::lastAdd = 0;
#endif