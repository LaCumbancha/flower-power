#ifndef FLOWER_POWER_STATSMENU_H
#define FLOWER_POWER_STATSMENU_H


#include "Menu.h"

class StatsMenu : public Menu {

public:
    explicit StatsMenu() = default;
    int show() override;

private:
    void display() override;

};


#endif //FLOWER_POWER_STATSMENU_H
