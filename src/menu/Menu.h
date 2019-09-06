#ifndef FLOWER_POWER_MENU_H
#define FLOWER_POWER_MENU_H

#include <iostream>

using namespace std;

class Menu {

public:
    virtual int run();

protected:
    virtual void display();

    static int readInt() {
        int option;
        cin >> option;
        return option;
    }

    static string readStr() {
        string option;
        getline(cin, option);
        return option;
    }
};

#endif //FLOWER_POWER_MENU_H
