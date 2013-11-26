/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "SoundTest.h"

static SoundTestCase suite_SoundTestCase;

static CxxTest::List Tests_SoundTestCase = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_SoundTestCase( "SoundTest.h", 11, "SoundTestCase", suite_SoundTestCase, Tests_SoundTestCase );

static class TestDescription_SoundTestCase_testSound : public CxxTest::RealTestDescription {
public:
 TestDescription_SoundTestCase_testSound() : CxxTest::RealTestDescription( Tests_SoundTestCase, suiteDescription_SoundTestCase, 25, "testSound" ) {}
 void runTest() { suite_SoundTestCase.testSound(); }
} testDescription_SoundTestCase_testSound;

