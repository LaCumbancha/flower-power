//
// Created by darius on 15/9/19.
//

#include <utility>

#include "iostream"
#include "gtest/gtest.h"
#include "../src/core/config/data/Seller.h"
#include "../src/core/config/Config.h"

class SellerTests : public  testing::Test {

public:
    Config config;

    SellerTests() = default;

    void SetUp() override {
        config = Config();
    }

    void TearDown() override {

    }
};

TEST_F(SellerTests, Sellers_data_is_loaded_properly) {

    config.loadData();
    auto salePoints = config.getSalePoints();
    ASSERT_EQ(salePoints.at(0).sellerName, "Tulipanmania");
    ASSERT_EQ(salePoints.at(1).sellerName, "La Floristeria Rosa");
    ASSERT_EQ(salePoints.at(2).sellerName, "Tu jardin");
    ASSERT_EQ(salePoints.at(3).sellerName, "La rosa azul");
    ASSERT_EQ(salePoints.at(4).sellerName, "El loto blanco");
    ASSERT_EQ(salePoints.at(5).sellerName, "El loto rojo");
    ASSERT_EQ(salePoints.at(6).sellerName, "Holanda tulipanes");

}
