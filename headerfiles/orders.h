#ifndef ORDERS_H
#define ORDERS_H
using namespace std;

class Orders {
    protected:
        int orderId;
        int medicineId;
        int quantity;
        string deliveryLocation;
        static int lastAdd;
    public:
        Orders(int, int, string);
        int getOrderId();
        void displayDetails();
};

#endif