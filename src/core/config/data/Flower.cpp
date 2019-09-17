#include "Flower.h"

Flower::Flower(int producerId, std::string producerName) :
    producerId(producerId), producerName(std::move(producerName)) {}