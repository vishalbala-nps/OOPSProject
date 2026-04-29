#ifndef MEDICINE_H
#define MEDICINE_H
#include <iostream>
using namespace std;

class Medicine {
    protected:
        int id;
        string name;
        double price;
        static int lastAdd;
    public:
        Medicine(string name, double price) {
            this->name = name;
            this->price = price;
            this->id = ++lastAdd;
        }
        int getId() {
            return id;
        }
        virtual void displayDetails() {
            cout << "Medicine ID: " << id << endl;
            cout << "Name: " << name << endl;
            cout << "Price: Rs." << price << endl;
        }

};

class OTCMedicine : public Medicine {
    public:
        OTCMedicine(string name, double price) : Medicine(name, price) {}
        void displayDetails() override {
            Medicine::displayDetails();
            cout << "Type: Over-the-Counter" << endl;
        }
};

class PrescriptionMedicine : public Medicine {
    public:
        PrescriptionMedicine(string name, double price) : Medicine(name, price) {}
        void displayDetails() override {
            Medicine::displayDetails();
            cout << "Type: Prescription" << endl;
        }
};

int Medicine::lastAdd = 0;
#endif