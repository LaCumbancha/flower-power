//
// Created by darius on 14/9/19.
//

#ifndef FLOWER_POWER_SELLER_H
#define FLOWER_POWER_SELLER_H


#include <string>

class Seller {
public:
    explicit Seller(int sellerId, std::string sellerName, int rosesStock, int tulipsStock);
    int sellerId;
    std::string sellerName;
    int rosesStock;
    int tulipsStock;

    std::string serialize();
    static Seller deserialize(std::string data);
};


#endif //FLOWER_POWER_SELLER_H
