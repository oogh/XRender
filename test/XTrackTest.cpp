//
// Created by Andy on 2020/12/18.
//

#include <gtest/gtest.h>
#define private public  // hack complier
#define protected public

#include "XTrack.hpp" // all private/protected members now become public!

#undef private
#undef protected


class XTrackTest : public ::testing::Test {
protected:
    static void SetUpTestCase() {
        std::cout << "+++XTrackTest start+++" << std::endl;
    }
    static void TearDownTestCase() {
        std::cout << "---XTrackTest end---" << std::endl;
    }
};

TEST_F(XTrackTest, createTest) {
    auto track = std::make_unique<XTrack>();
    EXPECT_LE(track->mId, 0);
}


