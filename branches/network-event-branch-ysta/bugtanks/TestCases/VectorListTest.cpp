/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "VectorListTest.h"

static VectorListTest suite_VectorListTest;

static CxxTest::List Tests_VectorListTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_VectorListTest( "VectorListTest.h", 6, "VectorListTest", suite_VectorListTest, Tests_VectorListTest );

static class TestDescription_VectorListTest_testAdd : public CxxTest::RealTestDescription {
public:
 TestDescription_VectorListTest_testAdd() : CxxTest::RealTestDescription( Tests_VectorListTest, suiteDescription_VectorListTest, 12, "testAdd" ) {}
 void runTest() { suite_VectorListTest.testAdd(); }
} testDescription_VectorListTest_testAdd;

static class TestDescription_VectorListTest_testAddWithInitialSize : public CxxTest::RealTestDescription {
public:
 TestDescription_VectorListTest_testAddWithInitialSize() : CxxTest::RealTestDescription( Tests_VectorListTest, suiteDescription_VectorListTest, 32, "testAddWithInitialSize" ) {}
 void runTest() { suite_VectorListTest.testAddWithInitialSize(); }
} testDescription_VectorListTest_testAddWithInitialSize;

static class TestDescription_VectorListTest_testRemoveFirst : public CxxTest::RealTestDescription {
public:
 TestDescription_VectorListTest_testRemoveFirst() : CxxTest::RealTestDescription( Tests_VectorListTest, suiteDescription_VectorListTest, 49, "testRemoveFirst" ) {}
 void runTest() { suite_VectorListTest.testRemoveFirst(); }
} testDescription_VectorListTest_testRemoveFirst;

static class TestDescription_VectorListTest_testRemoveLast : public CxxTest::RealTestDescription {
public:
 TestDescription_VectorListTest_testRemoveLast() : CxxTest::RealTestDescription( Tests_VectorListTest, suiteDescription_VectorListTest, 65, "testRemoveLast" ) {}
 void runTest() { suite_VectorListTest.testRemoveLast(); }
} testDescription_VectorListTest_testRemoveLast;

static class TestDescription_VectorListTest_testChangeValue : public CxxTest::RealTestDescription {
public:
 TestDescription_VectorListTest_testChangeValue() : CxxTest::RealTestDescription( Tests_VectorListTest, suiteDescription_VectorListTest, 81, "testChangeValue" ) {}
 void runTest() { suite_VectorListTest.testChangeValue(); }
} testDescription_VectorListTest_testChangeValue;

static class TestDescription_VectorListTest_testMax : public CxxTest::RealTestDescription {
public:
 TestDescription_VectorListTest_testMax() : CxxTest::RealTestDescription( Tests_VectorListTest, suiteDescription_VectorListTest, 95, "testMax" ) {}
 void runTest() { suite_VectorListTest.testMax(); }
} testDescription_VectorListTest_testMax;

static class TestDescription_VectorListTest_testClear : public CxxTest::RealTestDescription {
public:
 TestDescription_VectorListTest_testClear() : CxxTest::RealTestDescription( Tests_VectorListTest, suiteDescription_VectorListTest, 108, "testClear" ) {}
 void runTest() { suite_VectorListTest.testClear(); }
} testDescription_VectorListTest_testClear;

