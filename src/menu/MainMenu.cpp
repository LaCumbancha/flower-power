#include "MainMenu.h"

int MainMenu::show() {
    int option = 0;
    display();

    while (option != 3) {
        option = readInt();
        switch (option) {
            case 1:
                coreSystem.run();
                cout << endl;
                display();
                break;
            case 2:
                settingsMenu.show();
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
    cout << "[1] Run system" << endl;
    cout << "[2] Settings" << endl;
    cout << "[3] Exit" << endl;
    cout << "Selection: ";
}
