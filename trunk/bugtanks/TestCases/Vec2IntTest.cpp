/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "Vec2IntTest.h"

static Vec2IntTest suite_Vec2IntTest;

static CxxTest::List Tests_Vec2IntTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_Vec2IntTest( "Vec2IntTest.h", 6, "Vec2IntTest", suite_Vec2IntTest, Tests_Vec2IntTest );

static class TestDescription_Vec2IntTest_testEquality : public CxxTest::RealTestDescription {
public:
 TestDescription_Vec2IntTest_testEquality() : CxxTest::RealTestDescription( Tests_Vec2IntTest, suiteDescription_Vec2IntTest, 10, "testEquality" ) {}
 void runTest() { suite_Vec2IntTest.testEquality(); }
} testDescription_Vec2IntTest_testEquality;

static class TestDescription_Vec2IntTest_testDiffer : public CxxTest::RealTestDescription {
public:
 TestDescription_Vec2IntTest_testDiffer() : CxxTest::RealTestDescription( Tests_Vec2IntTest, suiteDescription_Vec2IntTest, 17, "testDiffer" ) {}
 void runTest() { suite_Vec2IntTest.testDiffer(); }
} testDescription_Vec2IntTest_testDiffer;

static class TestDescription_Vec2IntTest_testDifference : public CxxTest::RealTestDescription {
public:
 TestDescription_Vec2IntTest_testDifference() : CxxTest::RealTestDescription( Tests_Vec2IntTest, suiteDescription_Vec2IntTest, 24, "testDifference" ) {}
 void runTest() { suite_Vec2IntTest.testDifference(); }
} testDescription_Vec2IntTest_testDifference;

static class TestDescription_Vec2IntTest_testLongerThan : public CxxTest::RealTestDescription {
public:
 TestDescription_Vec2IntTest_testLongerThan() : CxxTest::RealTestDescription( Tests_Vec2IntTest, suiteDescription_Vec2IntTest, 32, "testLongerThan" ) {}
 void runTest() { suite_Vec2IntTest.testLongerThan(); }
} testDescription_Vec2IntTest_testLongerThan;

