/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "Vector2DTest.h"

static Vector2DTest suite_Vector2DTest;

static CxxTest::List Tests_Vector2DTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_Vector2DTest( "Vector2DTest.h", 6, "Vector2DTest", suite_Vector2DTest, Tests_Vector2DTest );

static class TestDescription_Vector2DTest_testSet : public CxxTest::RealTestDescription {
public:
 TestDescription_Vector2DTest_testSet() : CxxTest::RealTestDescription( Tests_Vector2DTest, suiteDescription_Vector2DTest, 22, "testSet" ) {}
 void runTest() { suite_Vector2DTest.testSet(); }
} testDescription_Vector2DTest_testSet;

static class TestDescription_Vector2DTest_testShiftLeft : public CxxTest::RealTestDescription {
public:
 TestDescription_Vector2DTest_testShiftLeft() : CxxTest::RealTestDescription( Tests_Vector2DTest, suiteDescription_Vector2DTest, 34, "testShiftLeft" ) {}
 void runTest() { suite_Vector2DTest.testShiftLeft(); }
} testDescription_Vector2DTest_testShiftLeft;

static class TestDescription_Vector2DTest_testShiftRight : public CxxTest::RealTestDescription {
public:
 TestDescription_Vector2DTest_testShiftRight() : CxxTest::RealTestDescription( Tests_Vector2DTest, suiteDescription_Vector2DTest, 112, "testShiftRight" ) {}
 void runTest() { suite_Vector2DTest.testShiftRight(); }
} testDescription_Vector2DTest_testShiftRight;

static class TestDescription_Vector2DTest_testShiftUp : public CxxTest::RealTestDescription {
public:
 TestDescription_Vector2DTest_testShiftUp() : CxxTest::RealTestDescription( Tests_Vector2DTest, suiteDescription_Vector2DTest, 192, "testShiftUp" ) {}
 void runTest() { suite_Vector2DTest.testShiftUp(); }
} testDescription_Vector2DTest_testShiftUp;

static class TestDescription_Vector2DTest_testShiftDown : public CxxTest::RealTestDescription {
public:
 TestDescription_Vector2DTest_testShiftDown() : CxxTest::RealTestDescription( Tests_Vector2DTest, suiteDescription_Vector2DTest, 259, "testShiftDown" ) {}
 void runTest() { suite_Vector2DTest.testShiftDown(); }
} testDescription_Vector2DTest_testShiftDown;

static class TestDescription_Vector2DTest_testExceptions : public CxxTest::RealTestDescription {
public:
 TestDescription_Vector2DTest_testExceptions() : CxxTest::RealTestDescription( Tests_Vector2DTest, suiteDescription_Vector2DTest, 326, "testExceptions" ) {}
 void runTest() { suite_Vector2DTest.testExceptions(); }
} testDescription_Vector2DTest_testExceptions;

