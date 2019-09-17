//
// Created by darius on 15/9/19.
//

#include <utility>

#include "iostream"
#include "gtest/gtest.h"
#include "../src/core/config/data/Seller.h"
#include "../src/core/config/Config.h"
#include "../src/core/job/seller/SellerJob.h"

class SellerTests : public  testing::Test {

public:
    Config config;
    SellerJob * sellerJob;

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

    ASSERT_EQ(salePoints.at(0).sellerId, 1);
    ASSERT_EQ(salePoints.at(1).sellerId, 2);
    ASSERT_EQ(salePoints.at(2).sellerId, 3);
    ASSERT_EQ(salePoints.at(3).sellerId, 4);
    ASSERT_EQ(salePoints.at(4).sellerId, 5);
    ASSERT_EQ(salePoints.at(5).sellerId, 6);
    ASSERT_EQ(salePoints.at(6).sellerId, 7);

    ASSERT_EQ(salePoints.at(0).sellerName, "Tulipanmania");
    ASSERT_EQ(salePoints.at(1).sellerName, "La Floristeria Rosa");
    ASSERT_EQ(salePoints.at(2).sellerName, "Tu jardin");
    ASSERT_EQ(salePoints.at(3).sellerName, "La rosa azul");
    ASSERT_EQ(salePoints.at(4).sellerName, "El loto blanco");
    ASSERT_EQ(salePoints.at(5).sellerName, "El loto rojo");
    ASSERT_EQ(salePoints.at(6).sellerName, "Holanda tulipanes");
}

TEST_F(SellerTests, Seller_job_responds_to_client_simulator) {
    Seller seller = Seller(666, "Las flores alucinógenas", 50, 100);
    sellerJob = new SellerJob(0, seller,0 ,0);
    sellerJob->run();
    delete(sellerJob);
}