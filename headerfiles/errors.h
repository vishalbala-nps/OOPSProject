#include <iostream>
#include <exception>
using namespace std;

class InvalidNoException : public exception {
    public: 
        const char* what() const throw() {
            return "Invalid option selected!";
        }
};