// test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <stdio.h>

#include <gtest/gtest.h>

#include "gtesthelper.h"
#include "application.h"

int main(int argc, char **argv)
{
  //register command line arguments with the application
  Application & app = Application::getInstance();
  app.init(argc, argv);

  //define default values for xml output report
  gTestHelper & hlp = gTestHelper::getInstance();
  if (hlp.isProcessorX86())
  {
    if (hlp.isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:msbuildreorderTest.x86.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:msbuildreorderTest.x86.release.xml";
  }
  else if (hlp.isProcessorX64())
  {
    if (hlp.isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:msbuildreorderTest.x64.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:msbuildreorderTest.x64.release.xml";
  }

  ::testing::GTEST_FLAG(filter) = "*";
  ::testing::InitGoogleTest(&argc, argv);

  int wResult = RUN_ALL_TESTS(); //Find and run all tests
  return wResult; // returns 0 if all the tests are successful, or 1 otherwise
}
