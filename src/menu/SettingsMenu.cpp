#include <string>
#include "SettingsMenu.h"

SettingsMenu::SettingsMenu(Config *config) {
    this->config = config;
}

int SettingsMenu::show() {
    int option = -1;
    display();

    while (option != 0) {
        option = readInt();
        switch (option) {
            case 0:
                break;
            case 1:
                cout << "Insert new distributions centers: ";
                this->config->setDistributionCenters(readInt());
                display();
                break;
            case 2:
                cout << "Insert new sale points: ";
                this->config->setSalePoints(readInt());
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
    cout << "[1] Distribution centers: " << this->config->getDistributionCenters() << endl;
    cout << "[2] Sale points: " << this->config->getSalePoints() << endl;
    cout << "What do you want to change? [0 for quit] ";
}
