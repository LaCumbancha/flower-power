
#include <utility>

#include "iostream"
#include "gtest/gtest.h"
#include "../src/core/config/data/Seller.h"
#include "../src/simulator/ClientSimulator.h"

class ClientSiulatorTests : public  testing::Test {

public:
    ClientSimulator * cs;
    void SetUp() override {
        cs = new ClientSimulator(0, 0, 0, 0);
    }

    void TearDown() override {
        delete(cs);
    }
};

TEST_F(ClientSiulatorTests, DistributionPipesAmountIsOk ) {
    cs->Run();
}
