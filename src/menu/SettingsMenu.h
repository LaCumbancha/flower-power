#ifndef FLOWER_POWER_SETTINGSMENU_H
#define FLOWER_POWER_SETTINGSMENU_H

#include "Menu.h"

class SettingsMenu: public Menu {

public:
    int show() override ;

private:
    int distributionCenters = 1;
    int salePoints = 1;

    void display() override;
    void customizeSettings();
};


#endif //FLOWER_POWER_SETTINGSMENU_H
