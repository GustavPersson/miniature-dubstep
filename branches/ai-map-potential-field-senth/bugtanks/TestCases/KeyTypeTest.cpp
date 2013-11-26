/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "KeyTypeTest.h"

static KeyTypeTest suite_KeyTypeTest;

static CxxTest::List Tests_KeyTypeTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_KeyTypeTest( "KeyTypeTest.h", 7, "KeyTypeTest", suite_KeyTypeTest, Tests_KeyTypeTest );

static class TestDescription_KeyTypeTest_testOwner : public CxxTest::RealTestDescription {
public:
 TestDescription_KeyTypeTest_testOwner() : CxxTest::RealTestDescription( Tests_KeyTypeTest, suiteDescription_KeyTypeTest, 24, "testOwner" ) {}
 void runTest() { suite_KeyTypeTest.testOwner(); }
} testDescription_KeyTypeTest_testOwner;

