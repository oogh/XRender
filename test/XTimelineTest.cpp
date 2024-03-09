//
// Created by Andy on 2020/12/18.
//

#include <gtest/gtest.h>
#include "XTimeline.hpp"

class XTimelineTest : public ::testing::Test {
protected:
    static void SetUpTestCase() {
        std::cout << "+++XTimelineTest start+++" << std::endl;
    }
    static void TearDownTestCase() {
        std::cout << "---XTimelineTest end---" << std::endl;
    }
};

TEST_F(XTimelineTest, openTest) {
    
}

