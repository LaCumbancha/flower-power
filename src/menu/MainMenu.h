#ifndef FLOWER_POWER_MAINMENU_H
#define FLOWER_POWER_MAINMENU_H


#include <sys/wait.h>
#include "Menu.h"
#include "SettingsMenu.h"
#include "../core/System.h"
#include "StatsMenu.h"

using namespace std;

class MainMenu: public Menu {

public:
    explicit MainMenu() = default;
    int show() override;

private:
    System* coreSystem = new System();
    SettingsMenu* settingsMenu = new SettingsMenu(coreSystem->getConfig());
    StatsMenu* statsMenu = new StatsMenu();

    void display() override;
};


#endif //FLOWER_POWER_MAINMENU_H
