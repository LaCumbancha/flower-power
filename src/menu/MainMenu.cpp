#include "MainMenu.h"

int MainMenu::show() {
    int option = -1;
    pid_t pid;
    display();

    while (option != 0) {
        option = readInt();
        switch (option) {
            case 0:
                cout << "Thanks for using Flower Power System!" << endl;
                break;
            case 1:
                pid = coreSystem.run();

                // Quitting if its child process.
                if (pid == CHILD_PROCESS_PID) {
                    option = 0;
                } else {
                    cout << endl;
                    display();
                }
                break;
            case 2:
                settingsMenu.show();
                cout << endl;
                display();
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
    cout << "Selection: [0 for quit] ";
}
