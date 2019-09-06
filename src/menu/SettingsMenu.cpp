#include <string>
#include "SettingsMenu.h"

int SettingsMenu::run() {
    int option = -1;
    display();

    while (option != 0) {
        option = readInt();
        switch (option) {
            case 0:
                break;
            case 1:
                cout << "Insert new distributions centers: ";
                this->distributionCenters = readInt();
                display();
                break;
            case 2:
                cout << "Insert new sale points: ";
                this->salePoints = readInt();
                display();
                break;
            default:
                cout << "Wrong option. Retry: ";
                break;
        }
    }

    return EXIT_SUCCESS;
}

void SettingsMenu::display() {
    cout << endl;
    cout << "Current settings:" << endl;
    cout << "[1] Distribution centers: " << distributionCenters << endl;
    cout << "[2] Sale points: " << salePoints << endl;
    cout << "What do you want to change? [0 for quit] ";
}
