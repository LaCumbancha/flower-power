#ifndef FLOWER_POWER_MAINMENU_H
#define FLOWER_POWER_MAINMENU_H

#include "Menu.h"
#include "SettingsMenu.h"

using namespace std;

class MainMenu: public Menu {

public:
    int run() override;

private:
    SettingsMenu settingsMenu = SettingsMenu();

    void display() override;
};


#endif //FLOWER_POWER_MAINMENU_H
