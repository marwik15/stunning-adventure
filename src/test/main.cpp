#include <gtest/gtest.h>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

#define test
#include <SA/stats.hpp>

TEST(SA, mergeUpperCaseFail) {

	Statistics stat(true);
	ASSERT_DEATH(stat.count(std::string("d")), "");
}

TEST(SA, mergeUpperCase) {

	Statistics stat(false);
	stat.printFileInfo();
	ASSERT_DEATH(stat.count(std::string("d")), "");
}

int main(int argc, char** argv) {
	InitGoogleTest(&argc, argv);
	UnitTest& unit_test = *UnitTest::GetInstance();
	return RUN_ALL_TESTS();
}