// msbuildreorder.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include "BuildLogManager.h"

#include <stdio.h>
//#include <tchar.h>
#include <string>

bool fileExist(const char * iFilePath)
{
  FILE * f = fopen(iFilePath, "rb");
  if (f)
  {
    fclose(f);
    return true;
  }
  return false;
}

void printHeader()
{
  printf("msbuildreorder v1.0\n");
}

void printUsage()
{
  printHeader();
  printf("  msbuildreorder.exe [input file] [output file]\n");
  printf("    [input file]:  Build log file to reorder\n");
  printf("    [output file]: Reordered build log file\n");
  printf("  Note: Input and output files can be the same for an in-place reordering\n");
  printf("\n");
}

int main(int argc, char* argv[])
{
  //parse arguments
  if (argc != 3)
  {
    printUsage();
    exit(1);
  }
  const char * inputFilePath = argv[1];
  const char * outputFilePath = argv[2];

  printHeader();

  //check input file
  if (!fileExist(inputFilePath))
  {
    printf("Error. File \"%s\" does not exists.\n", inputFilePath);
    exit(2);
  }

  //load
  BuildLogManager m;
  StringList content;
  if (!m.loadBuildLog(inputFilePath, content))
  {
    printf("Error. Unable to load build log file \"%s\".\n", inputFilePath);
    exit(3);
  }

  //reorder
  if (!m.reorderBuildLog(content))
  {
    printf("Error. Failed to reorder build log \"%s\". Not a build log ?\n", inputFilePath);
    exit(4);
  }

  //save back to file
  if (!m.saveBuildLog(outputFilePath, content))
  {
    printf("Error. Failed to save reordered build log to \"%s\".\n", outputFilePath);
    exit(4);
  }

  //success
  if (std::string(inputFilePath) == std::string(outputFilePath))
    printf("Reorder of \"%s\" completed sucesfully.\n", inputFilePath);
  else
    printf("Reorder of \"%s\" into \"%s\" completed sucesfully.\n", inputFilePath, outputFilePath);
	return 0;
}
