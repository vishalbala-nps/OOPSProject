#include "medicine.h"
#include <iostream>
using namespace std;

Medicine::Medicine(string name, double price) {
    this->name = name;
    this->price = price;
    this->id = ++lastAdd;
}

int Medicine::getId() {
    return id;
}

string Medicine::getMedicine() {
    return name;
}

double Medicine::getPrice() { 
    return price; 
}

void Medicine::displayDetails() {
    cout << "  * Medicine ID : " << id << endl;
    cout << "    Name        : " << name << endl;
    cout << "    Price       : Rs." << price << endl;
}

void OTCMedicine::displayDetails() {
    Medicine::displayDetails();
    cout << "    Type        : Over-the-Counter" << endl;
}

void PrescriptionMedicine::displayDetails() {
    Medicine::displayDetails();
    cout << "    Type        : Prescription" << endl;
}