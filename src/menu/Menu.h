#ifndef FLOWER_POWER_MENU_H
#define FLOWER_POWER_MENU_H
#define CHILD_PROCESS_PID 0


#include <iostream>

using namespace std;

class Menu {

public:
    virtual int show();

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
