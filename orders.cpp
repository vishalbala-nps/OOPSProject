#include <iostream>
#include "orders.h"
using namespace std;

Orders::Orders(int medicineId, int quantity,string deliveryLocation) {
    this->medicineId = medicineId;
    this->quantity = quantity;
    this->orderId = ++lastAdd;
    this->deliveryLocation = deliveryLocation;
}

int Orders::getOrderId() {
    return orderId;
}

void Orders::displayDetails() {
    cout << "  * Order ID          : " << orderId << endl;
    cout << "    Medicine ID       : " << medicineId << endl;
    cout << "    Quantity          : " << quantity << endl;
    cout << "    Delivery Location : " << deliveryLocation << endl;
}