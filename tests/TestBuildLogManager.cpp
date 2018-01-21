#include "TestBuildLogManager.h"
#include "gtesthelper.h"
#include "application.h"

#include "BuildLogManager.h"

gTestHelper & hlp = gTestHelper::getInstance();

std::string getTestFilePath(const std::string & iName)
{
  std::string file;
  file.append("test_files\\");
  file.append(hlp.getTestQualifiedName());
  file.append(".");
  file.append(iName);
  file.append(".txt");
  return file;
}

std::string getExpectedFilePath()
{
  return getTestFilePath("expected");
}

std::string getActualFilePath()
{
  return getTestFilePath("actual");
}

std::string getInputFilePath()
{
  return getTestFilePath("input");
}

void TestBuildLogManager::SetUp()
{

}

void TestBuildLogManager::TearDown()
{
}

TEST_F(TestBuildLogManager, testLibprotobufLog)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string inputFilePath    = getInputFilePath();
  static const std::string actualFilePath   = getActualFilePath();

  ASSERT_TRUE(hlp.fileExists(inputFilePath.c_str()));
  ASSERT_TRUE(hlp.fileExists(expectedFilePath.c_str()));

  BuildLogManager mgr;

  StringList content; 
  ASSERT_TRUE(mgr.loadBuildLog(inputFilePath.c_str(), content));
  ASSERT_TRUE(mgr.reorderBuildLog(content));
  ASSERT_TRUE(mgr.saveBuildLog(actualFilePath.c_str(), content));

  ASSERT_TRUE(hlp.fileExists(actualFilePath.c_str()));

  //assert files are equals
  std::string errorMessage;
  bool fileEquals = hlp.isFileEquals(actualFilePath.c_str(), expectedFilePath.c_str(), errorMessage);
  ASSERT_TRUE(fileEquals) << errorMessage.c_str();

}
