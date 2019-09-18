#ifndef FLOWER_POWER_SELLERREQUEST_H
#define FLOWER_POWER_SELLERREQUEST_H


#include <string>
#include <utility>
#include <vector>

class SellerRequest {
public:
    int rosesBoxAmount;
    int tulipsBoxAmount;

    explicit SellerRequest(std::string sellerId, int rosesBoxAmount, int tulipsBoxAmount) :
            sellerId(std::move(sellerId)), rosesBoxAmount(rosesBoxAmount), tulipsBoxAmount(tulipsBoxAmount) {};
    std::string sellerId;

    std::string serialize();

    static SellerRequest deserialize(const std::string &data);
};


#endif //FLOWER_POWER_SELLERREQUEST_H
