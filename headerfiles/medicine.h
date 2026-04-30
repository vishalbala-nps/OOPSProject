#ifndef MEDICINE_H
#define MEDICINE_H
#include <iostream>
#include <string>
using namespace std;

class Medicine {
    protected:
        int id;
        string name;
        double price;
        static int lastAdd;
    public:
        Medicine(string, double);
        int getId();
        string getMedicine();
        double getPrice();
        virtual void displayDetails();
};

class OTCMedicine : public Medicine {
    public:
        OTCMedicine(string name, double price) : Medicine(name, price) {}
        void displayDetails() override;
};

class PrescriptionMedicine : public Medicine {
    public:
        PrescriptionMedicine(string name, double price) : Medicine(name, price) {}
        void displayDetails() override;
};

#endif