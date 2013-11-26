/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ParenPrinter.h>

int main() {
 return CxxTest::ParenPrinter().run();
}
#include "main.h"

static StubTestCase suite_StubTestCase;

static CxxTest::List Tests_StubTestCase = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_StubTestCase( "main.h", 4, "StubTestCase", suite_StubTestCase, Tests_StubTestCase );

static class TestDescription_StubTestCase_testStub : public CxxTest::RealTestDescription {
public:
 TestDescription_StubTestCase_testStub() : CxxTest::RealTestDescription( Tests_StubTestCase, suiteDescription_StubTestCase, 7, "testStub" ) {}
 void runTest() { suite_StubTestCase.testStub(); }
} testDescription_StubTestCase_testStub;

#include <cxxtest/Root.cpp>
