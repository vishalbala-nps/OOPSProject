#include <iostream>
#include "orders.h"
using namespace std;

Orders::Orders(int medicineId, int quantity,string deliveryLocation) {
    this->medicineId = medicineId;
    this->quantity = quantity;
    this->orderId = ++lastAdd;
    this->deliveryLocation = deliveryLocation;
    this->status = PROCESSING;
}

int Orders::getOrderId() {
    return orderId;
}

void Orders::setStatus(OrderStatus status) {
    this->status = status;
}

OrderStatus Orders::getStatus() {
    return status;
}

void Orders::displayDetails() {
    cout << "  * Order ID          : " << orderId << endl;
    cout << "    Medicine ID       : " << medicineId << endl;
    cout << "    Quantity          : " << quantity << endl;
    cout << "    Delivery Location : " << deliveryLocation << endl;
    cout << "    Status            : ";
    if (status == PROCESSING) cout << "Processing" << endl;
    else if (status == SHIPPED) cout << "Shipped" << endl;
    else if (status == DELIVERED) cout << "Delivered" << endl;
}