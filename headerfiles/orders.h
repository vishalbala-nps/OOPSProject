#ifndef ORDERS_H
#define ORDERS_H
#include <iostream>
using namespace std;

class Orders {
    protected:
        int orderId;
        int medicineId;
        int quantity;
        string deliveryLocation;
        static int lastAdd;
    public:
        Orders(int userId, int medicineId, int quantity,string deliveryLocation) {
            this->medicineId = medicineId;
            this->quantity = quantity;
            this->orderId = ++lastAdd;
            this->deliveryLocation = deliveryLocation;
        }
        int getOrderId() {
            return orderId;
        }
        void displayDetails() {
            cout << "  * Order ID          : " << orderId << endl;
            cout << "    Medicine ID       : " << medicineId << endl;
            cout << "    Quantity          : " << quantity << endl;
            cout << "    Delivery Location : " << deliveryLocation << endl;
        }
};

int Orders::lastAdd = 0;
#endif