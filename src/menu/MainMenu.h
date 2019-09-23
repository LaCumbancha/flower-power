#ifndef FLOWER_POWER_MAINMENU_H
#define FLOWER_POWER_MAINMENU_H


#include <sys/wait.h>
#include "Menu.h"
#include "SettingsMenu.h"
#include "StatsMenu.h"
#include "../core/System.h"
#include "../utils/ProcessKiller.h"


enum SYSTEM_STATUS { NONE, RUNNING1, RUNNING2, STOPPED };

using namespace std;

class MainMenu: public Menu {

public:
    explicit MainMenu() = default;
    int show() override;

private:
    System* coreSystem = new System();
    SettingsMenu* settingsMenu = new SettingsMenu(coreSystem->getConfig());
    StatsMenu* statsMenu = new StatsMenu();
    SYSTEM_STATUS status = NONE;


    void display() override;
    string systemStart();
};


#endif //FLOWER_POWER_MAINMENU_H
