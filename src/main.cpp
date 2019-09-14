#include "menu/MainMenu.h"
#include "utils/Logger.h"

using namespace std;

int main(int argc, char *argv[]) {

    // Creating listener logger.
    pid_t pid = fork();
    if (pid == CHILD_PROCESS_PID) {
        Logger::run();
    }

    Logger::writing();
    Logger::info("Initilized logger in process with PID #" + std::to_string(pid));
    MainMenu menu = MainMenu();
    menu.show();

    Logger::info("Main program finished.");
    Logger::close();
    wait(nullptr);
    return EXIT_SUCCESS;
}
