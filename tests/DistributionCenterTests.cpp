//
// Created by darius on 15/9/19.
//

#include <utility>

#include "iostream"
#include "gtest/gtest.h"
#include "../src/core/config/data/Seller.h"
#include "../src/core/config/Config.h"
#include "../src/core/DistributionCenter.h"

class DistributionCenterTests : public  testing::Test {

public:
    Config config;
    DistributionCenter * dc;

    void SetUp() override {
        config = Config();
        config.loadData();
        dc = new DistributionCenter(&config, 0);
    }

    void TearDown() override {
        delete(dc);
    }
};

TEST_F(DistributionCenterTests, DistributionPipesAmountIsOk ) {
    std::vector<Pipe*> distributionPipes = dc->getDistributionPipes();
    ASSERT_EQ(distributionPipes.size(), 7);
}
