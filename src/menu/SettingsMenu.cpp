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
            case 1: {
                cout << "Select new debug level: [0] DEBUG, [1] INFO, [2] WARN, [3] ERROR ";
                int logOption = readInt();
                while (logOption < 0 or logOption > 4) {
                    cout << "Wrong option. Retry: ";
                    logOption = readInt();
                }
                Logger::setLoggingLevel(logOption);
                display();
                break;
            }
            case 2: {
                cout << "Insert new distributions centers: ";
                this->config->setDistributionCenters(readInt());
                display();
                break;
            }
            case 3: {
                cout << "Insert new sale points: ";
                this->config->setSalePoints(readInt());
                display();
                break;
            }
            case 4: {
                cout << "Insert new clients: ";
                this->config->setClients(readInt());
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

void SettingsMenu::display() {
    cout << endl;
    cout << "Current settings:" << endl;
    cout << "[1] Logging level: " << Logger::getLoggingLevel() << endl;
    cout << "[2] Distribution centers: " << this->config->getDistributionCenters() << endl;
    cout << "[3] Sale points: " << this->config->getSalePoints() << endl;
    cout << "[4] Clients: " << this->config->getClients() << endl;
    cout << "What do you want to change? [0 for quit] ";
}
