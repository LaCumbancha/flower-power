#include "MainMenu.h"

int MainMenu::show() {
    int option = -1;
    display();

    while (option != 0) {
        option = readInt();
        switch (option) {
            case 0:
                cout << "Thanks for using Flower Power System!" << endl;
                break;
            case 1: {

                switch (this->status) {
                    case NONE: {
                        // Running system.
                        this->status = RUNNING1;
                        pid_t pid = fork();

                        if (pid == CHILD_PROCESS_PID) {
                            // System process.
                            coreSystem->run();
                            coreSystem->finish();
                        }

                        ProcessKiller::addPID(pid);
                        cout << endl;
                        display();
                        break;
                    }
                    case RUNNING1: {
                        // Stopping system.
                        ProcessKiller::killAll();
                        this->status = STOPPED;
                        cout << endl;
                        display();
                        break;
                    }
                    case RUNNING2: {
                        // Stopping system.
                        ProcessKiller::killAll();
                        this->status = STOPPED;
                        cout << endl;
                        display();
                        break;
                    }
                    case STOPPED: {
                        // Resuming system.
                        this->status = RUNNING1;
                        ContextStatus::loadDataForResume();
                        pid_t pid = fork();

                        if (pid == CHILD_PROCESS_PID) {
                            // System process.
                            coreSystem->run();
                            coreSystem->finish();
                        }

                        ProcessKiller::addPID(pid);
                        cout << endl;
                        display();
                        break;
                    }
                }

                break;

            }
            case 2:
                if (this->status == RUNNING1) this->status = RUNNING2;
                settingsMenu->show();
                cout << endl;
                display();
                break;
            case 3:
                if (this->status == RUNNING1) this->status = RUNNING2;
                statsMenu->show();
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
    cout << systemStart() << endl;
    cout << "[2] Settings" << endl;
    cout << "[3] Stats" << endl;
    cout << "Selection: [0 for quit] ";
}

std::string MainMenu::systemStart() {
    switch (this->status) {
        case NONE: return "[1] Run system";
        case RUNNING1: return "[1] Stop system";
        case RUNNING2: {
            if (ContextStatus::retrieveSystemFinished()) {
                this->status = NONE;
                return "[1] Run system";
            }
            return "[1] Stop system";
        }
        case STOPPED: return "[1] Resume system";
    }
}