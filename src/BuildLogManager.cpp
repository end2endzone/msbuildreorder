#include "BuildLogManager.h"
#include <algorithm>

BuildLogManager::BuildLogManager(void)
{
}

BuildLogManager::~BuildLogManager(void)
{
}

bool BuildLogManager::loadBuildLog(const char * iFilePath, StringList & oContent)
{
  oContent.clear();

  FILE * f = fopen(iFilePath, "r");
  if (!f)
    return false;

  static const int BUFFER_SIZE = 102400;
  char buffer[BUFFER_SIZE] = {0};
  while( fgets(buffer, BUFFER_SIZE-1, f) != NULL )
  {
    std::string line = buffer;
    oContent.push_back(line);
  }

  fclose(f);
  return true;
}

bool BuildLogManager::reorderBuildLog(StringList & ioContent)
{
  size_t intputContentSize = ioContent.size();

  struct LOG_ENTRY
  {
    size_t fileIndex;     //original order in the log file
    size_t projectIndex;  //value of entries that start like "34>"
    size_t section;       //increases everytime the logs switches from "xx>" to simple text and vice versa. Even for simple lines, ods for projects lines.
    std::string line;     

    //sorting support
    bool operator() (const LOG_ENTRY & i, const LOG_ENTRY & j)
    {
      //sort on section
      if (i.section == j.section)
      {
        //same section. sort on projectIndex
        if (i.projectIndex == j.projectIndex)
        {
          //same section & projectIndex, sort line number
          return (i.fileIndex < j.fileIndex);
        }
        else
          return (i.projectIndex < j.projectIndex);
      }
      else
        return i.section < j.section;
    }
  };
  typedef std::vector<LOG_ENTRY> LogEntryList;

  //first pass
  size_t numProjects = 0;
  LogEntryList entries;
  size_t currentSection = 0;
  for(size_t i=0; i<ioContent.size(); i++)
  {
    //copy content to a LOG_ENTRY
    LOG_ENTRY entry;
    entry.fileIndex = i;
    entry.line = ioContent[i];
    entry.projectIndex = findProjectIndex(entry.line);
    
    //find number of projects
    if (entry.projectIndex > numProjects)
      numProjects = entry.projectIndex;

    //compute section
    if (entry.projectIndex != 0 && currentSection%2 == 0)
      currentSection++;
    if (entry.projectIndex == 0 && currentSection%2 != 0)
      currentSection++;
    entry.section = currentSection;

    //move entry to LogEntries
    entries.push_back(entry);
  }

  //sort
  LOG_ENTRY tmp;
  std::sort (entries.begin(), entries.end(), tmp);

  //back to output entry
  ioContent.clear();
  for(size_t i=0; i<entries.size(); i++)
  {
    const LOG_ENTRY & entry = entries[i];
    ioContent.push_back(entry.line);
  }

  bool success = (intputContentSize == ioContent.size());
  return success;
}

bool BuildLogManager::saveBuildLog(const char * iFilePath, const StringList & iContent)
{
  FILE * f = fopen(iFilePath, "w");
  if (!f)
    return false;

  for(size_t i=0; i<iContent.size(); i++)
  {
    const std::string line = iContent[i];
    fputs(line.c_str(), f);
  }

  fclose(f);
  return true;
}

size_t BuildLogManager::findProjectIndex(std::string & iLine)
{
  for(size_t i=0; i<iLine.size()-1; i++)
  {
    char c = iLine[i];
    if ( (c >= '0' && c <= '9') || c == '>' )
    {
      //character is valid
      if (c == '>' && i >= 0)
      {
        //end of project index found.
        std::string projectIndexString = iLine.substr(0, i);
        size_t projectIndex = atol(projectIndexString.c_str());
        return projectIndex;
      }
    }
    else
    {
      //invalid character. Not a project log entry 
      return 0;
    }
  }
  return 0;
}
