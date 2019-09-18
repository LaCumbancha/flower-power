#ifndef FLOWER_POWER_SETTINGSMENU_H
#define FLOWER_POWER_SETTINGSMENU_H

#include <string>
#include "Menu.h"
#include "../utils/Logger.h"
#include "../core/config/Config.h"


class SettingsMenu: public Menu {

public:
    explicit SettingsMenu(Config *config);
    int show() override ;

private:
    Config* config;

    void display() override;
};


#endif //FLOWER_POWER_SETTINGSMENU_H
