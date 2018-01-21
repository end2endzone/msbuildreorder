#pragma once

#include <string>
#include <vector>

typedef std::vector<std::string> StringList;

class BuildLogManager
{
public:
  BuildLogManager(void);
  virtual ~BuildLogManager(void);

  bool loadBuildLog(const char * iFilePath, StringList & oContent);
  bool reorderBuildLog(StringList & ioContent);
  bool saveBuildLog(const char * iFilePath, const StringList & iContent);

private:
  size_t findProjectIndex(std::string & iLine);
};
