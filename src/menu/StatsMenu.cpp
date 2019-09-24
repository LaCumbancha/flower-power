#include "StatsMenu.h"
#include "../utils/StatsCenter.h"

int StatsMenu::show() {
    int option = -1;
    display();

    while (option != 0) {
        option = readInt();
        switch (option) {
            case 0:
                break;
            case 1: {
                cout << StatsCenter::getMostSoldProducer() << endl;
                display();
                break;
            }
            case 2: {
                cout << StatsCenter::getMostSoldFlower() << endl;
                display();
                break;
            }
            default: {
                cout << "Wrong option. Retry: ";
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}

void StatsMenu::display() {
    cout << endl;
    cout << "Available stats:" << endl;
    cout << "[1] Best selling producer" << endl;
    cout << "[2] Best selling flower type" << endl;
    cout << "What do you want to see? [0 for quit] ";
}
