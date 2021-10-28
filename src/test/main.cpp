#include <gtest/gtest.h>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

#include <string>
#include <sstream>

#define test
#include <SA/stats.hpp>
#include <SA/Lcounter.hpp>


TEST(SAlines, basicEmptyLineCount) {
	std::stringstream ss;

	ss << "not\n\n1";

	Lcounter lineCounter;
	lineCounter.countLines(ss);

	EXPECT_EQ(lineCounter.getEmptyLineCount(),1);
}

TEST(SAlines, basicNonEmptyLineCount) {
	std::stringstream ss;

	ss << "not\n\n1";

	Lcounter lineCounter;
	lineCounter.countLines(ss);

	EXPECT_EQ(lineCounter.getnonemptyLineCount(), 2);
}

TEST(SAlines, EmptyLineCount) {
	std::stringstream ss;

	ss << "not1\n\n\nnot2\n\nnot enter\nasd\n\n\n\n\n";

	Lcounter lineCounter;
	lineCounter.countLines(ss);

	EXPECT_EQ(lineCounter.getEmptyLineCount(), 7);
}

TEST(SAlines, nonemptyLineCount) {
	std::stringstream ss;

	ss << "not1\n\n\nnot2\n\nnot enter\nasd\n\n\n\n\n";

	Lcounter lineCounter;
	lineCounter.countLines(ss);

	EXPECT_EQ(lineCounter.getnonemptyLineCount(), 4);
}

TEST(SA, mergeUpperCase) {

	Statistics stat(true);
	ASSERT_DEATH(stat.count(std::string("d")), "");
}

int main(int argc, char** argv) {
	InitGoogleTest(&argc, argv);
	UnitTest& unit_test = *UnitTest::GetInstance();
	return RUN_ALL_TESTS();
}