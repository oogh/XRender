//
// Created by Andy on 2020/12/28.
//

#include <gtest/gtest.h>
#define private public  // hack complier
#define protected public

#include "XVideoCodec.hpp" // all private/protected members now become public!

#undef private
#undef protected


class XVideoCodecTest : public ::testing::Test {
protected:
  static void SetUpTestCase() {
    std::cout << "+++XVideoCodecTest start+++" << std::endl;
  }
  static void TearDownTestCase() {
    std::cout << "---XVideoCodecTest end---" << std::endl;
  }
};

TEST_F(XVideoCodecTest, createTest) {
  auto codec = std::make_unique<XVideoCodec>();
  EXPECT_TRUE(codec->mFilename.empty());
  EXPECT_EQ(codec->mIndex, -1);
  EXPECT_EQ(codec->mStatus, 0);
}

TEST_F(XVideoCodecTest, openTest) {
  auto codec = std::make_unique<XVideoCodec>();
  EXPECT_EQ(codec->open(), -22);

  codec->setFilename("/Users/andy/Workspace/Resources/jieqian.mp4");
  EXPECT_EQ(codec->open(), 0);
  codec->close();

  codec->setFilename("error_path");
  EXPECT_LE(codec->open(), 0);
  codec->close();
}