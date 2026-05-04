#include <string>
#ifndef ORDERS_H
#define ORDERS_H
using namespace std;

enum OrderStatus { PROCESSING, SHIPPED, DELIVERED };

class Orders {
protected:
  int orderId;
  int medicineId;
  int quantity;
  string deliveryLocation;
  OrderStatus status;
  static int lastAdd;

public:
  Orders(int, int, string);
  int getOrderId();
  void setStatus(OrderStatus);
  OrderStatus getStatus();
  void displayDetails();
};

#endif
