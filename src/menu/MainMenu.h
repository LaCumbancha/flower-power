#ifndef FLOWER_POWER_MAINMENU_H
#define FLOWER_POWER_MAINMENU_H

#include "Menu.h"
#include "SettingsMenu.h"
#include "../core/System.h"

using namespace std;

class MainMenu: public Menu {

public:
    int show() override;

private:
    System coreSystem = System();
    SettingsMenu settingsMenu = SettingsMenu(coreSystem.getConfig());

    void display() override;
};


#endif //FLOWER_POWER_MAINMENU_H
