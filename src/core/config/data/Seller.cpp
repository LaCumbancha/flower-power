//
// Created by darius on 14/9/19.
//

#include "Seller.h"

std::string Seller::serialize() {
    //TODO
    return "Seller serialize\n";
}

Seller Seller::deserialize(std::string data) {
    //TODO
    return Seller(9999, "Hardcoded seller", 666, 666);
}

Seller::Seller(int sellerId, std::string sellerName, int rosesStock, int tulipsStock) {
    this->sellerId = sellerId;
    this->sellerName = sellerName;
    this->rosesStock = rosesStock;
    this->tulipsStock = tulipsStock;
}
