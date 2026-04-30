#include <iostream>
#include <exception>
using namespace std;

class InvalidNoException : public exception {
    public: 
        const char* what() const throw() {
            return "Invalid option selected!";
        }
};

class MedicineNotFoundException : public exception {
    public:
        const char* what() const throw() {
            return "Medicine does not exist!";
        }
};

class OutOfStockException : public exception {
    public:
        const char* what() const throw() {
            return "Medicine is currently out of stock!";
        }
};