#ifndef FLOWER_POWER_STATSCENTER_H
#define FLOWER_POWER_STATSCENTER_H


#include "Pipe.h"
#include "Logger.h"
#include "../core/config/data/Flower.h"


class StatsCenter {

public:
    explicit StatsCenter() = default;

    static void run();
    static void addingMode();
    static void addSale(Flower flower, std::string flowerType);

    static std::string getMostSoldFlower();
    static std::string getMostSoldProducer();

private:
    static Pipe* _sellerPipe;
    static Pipe* _statsPipe;

    static bool isSaleIncoming(const std::string& data);
    static bool isRequestIncoming(const std::string& data);
    static bool isRoseIncoming(const std::string& flower);
    static bool isTulipIncoming(const std::string& flower);
    static bool isProducerRequest(const std::string& request);
    static bool isFlowerRequest(const std::string& request);

    static void updateStats(const std::string& flower);

    static void wroteStats(const std::string& request);
};


#endif //FLOWER_POWER_STATSCENTER_H
