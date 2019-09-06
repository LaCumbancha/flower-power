#include "MainMenu.h"

int MainMenu::run() {
    int option = 0;
    display();

    while (option != 3) {
        option = readInt();
        switch (option) {
            case 1:
                // systemMenu();
                cout << "Accessing System Menu" << endl;
                break;
            case 2:
                settingsMenu.run();
                cout << endl;
                display();
                break;
            case 3:
                cout << "Thanks for using Flower Power System!" << endl;
                break;
            default:
                cout << "Wrong option. Retry: ";
                break;
        }
    }

    return EXIT_SUCCESS;
}

void MainMenu::display() {
    cout << "Flower Power Menu!" << endl;
    cout << "Select your option:" << endl;
    cout << "[1] Enter system" << endl;
    cout << "[2] Settings" << endl;
    cout << "[3] Exit" << endl;
    cout << "Selection: ";
}
