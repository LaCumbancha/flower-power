//
// Created by darius on 18/9/19.
//

#ifndef FLOWER_POWER_CLASSIFIERBOX_H
#define FLOWER_POWER_CLASSIFIERBOX_H

#include <utility>
#include <vector>
#include "Flower.h"

#define CLASSIFIER_BOX_SIZE 100

/**
 * ClassifierBox
 * Box of 100 flowers of a single kind coming from
 * different producers
 */
class ClassifierBox {
private:
    FlowerType flowerType;
    std::vector<Flower> _flowers;

public:
    ClassifierBox(FlowerType  flowerType, std::vector<Flower> flowers) : flowerType(flowerType), _flowers(std::move(flowers)) {}

    std::string serialize();

    static ClassifierBox deserialize(const std::string& serializedCB);
};


#endif //FLOWER_POWER_CLASSIFIERBOX_H
