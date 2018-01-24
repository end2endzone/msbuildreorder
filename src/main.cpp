// msbuildreorder.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include "BuildLogManager.h"
#include "filesystemfunc.h"
#include "argumentparser.h"
#include "logger.h"

#include <stdio.h>
#include <string>

#include "..\version_info.h"

enum APP_ERROR_CODES
{
  APP_ERROR_SUCCESS,
  APP_ERROR_MISSINGARGUMENTS,
  APP_ERROR_INPUTFILE_NOTFOUND,
  APP_ERROR_INPUTFILE_NOT_LOADED,
  APP_ERROR_REORDER_FAILED,
  APP_ERROR_UNABLETOCREATEOUTPUTFILES,
};

const char * getErrorCodeDescription(const APP_ERROR_CODES & iErrorCode)
{
  switch(iErrorCode)
  {
  case APP_ERROR_SUCCESS:
    return "Success";
    break;
  case APP_ERROR_MISSINGARGUMENTS:
    return "Missing arguments";
    break;
  case APP_ERROR_INPUTFILE_NOTFOUND:
    return "Unable to open input file";
    break;
  case APP_ERROR_INPUTFILE_NOT_LOADED:
    return "Unable to load input file";
    break;
  case APP_ERROR_REORDER_FAILED:
    return "Failed to reorder build log. Not a build log?";
    break;
  case APP_ERROR_UNABLETOCREATEOUTPUTFILES:
    return "Failed to save output file";
    break;
  default:
    return "Unknown error";
  };
}

int getCopyrightYear()
{
  static const int DEFAULT_YEAR = 2016;
  std::string compilationDate = __DATE__;
  size_t lastSpace = compilationDate.find_last_of(" ");
  if (lastSpace == std::string::npos)
    return DEFAULT_YEAR;
  const char * yearStr = &compilationDate[lastSpace+1];
  int year = atoi(yearStr);
  return year;
}

void printHeader()
{
  printf("msbuildreorder v%s - Reorder Visual Studio build logs.\n", MSBUILDREORDER_VERSION);
  printf("Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", getCopyrightYear());
  printf("msbuildreorder is open source software, see http://github.com/end2endzone/bin2cpp \n");
}


void printUsage()
{
  //usage string in docopt format. See http://docopt.org/
  static const char usage[] = 
    "Usage:\n"
    "  msbuildreorder --input=<path> --output=<path>\n"
    "  msbuildreorder --help\n"
    "  msbuildreorder --version\n"
    "\n"
    "Options:\n"
    "  --help               Display this help message.\n"
    "  --version            Display this application version.\n"
    "  --input=<path>       Path of the input log file.\n"
    "  --output=<path>      Path of the reordered output log file. Note that output can be the same value as input for for an in-place reordering.\n"
    "\n";
  printf("%s", usage);
}

int main(int argc, char* argv[])
{
  //help
  std::string dummy;
  if (cmdline::parseArgument("help", dummy, argc, argv))
  {
    printHeader();
    printUsage();
    return APP_ERROR_SUCCESS;
  }

  //version
  if (cmdline::parseArgument("version", dummy, argc, argv))
  {
    printHeader();
    return APP_ERROR_SUCCESS;
  }

  //mandatory arguments
  std::string inputFilePath;
  std::string outputFilePath;

  //input
  if (!cmdline::parseArgument("input", inputFilePath, argc, argv))
  {
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    logger::log(logger::LOG_ERROR, "%s (input)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  //output
  if (!cmdline::parseArgument("output", outputFilePath, argc, argv))
  {
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    logger::log(logger::LOG_ERROR, "%s (output)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  //check input file validity
  if (!filesystem::fileExists(inputFilePath.c_str()))
  {
    APP_ERROR_CODES error = APP_ERROR_INPUTFILE_NOTFOUND;
    logger::log(logger::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), inputFilePath.c_str());
    return error;
  }

  //load
  BuildLogManager m;
  StringList content;
  if (!m.loadBuildLog(inputFilePath.c_str(), content))
  {
    APP_ERROR_CODES error = APP_ERROR_INPUTFILE_NOT_LOADED;
    logger::log(logger::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), inputFilePath.c_str());
    return error;
  }

  //reorder
  if (!m.reorderBuildLog(content))
  {
    APP_ERROR_CODES error = APP_ERROR_REORDER_FAILED;
    logger::log(logger::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), inputFilePath.c_str());
    return error;
  }

  //save back to file
  if (!m.saveBuildLog(outputFilePath.c_str(), content))
  {
    APP_ERROR_CODES error = APP_ERROR_UNABLETOCREATEOUTPUTFILES;
    logger::log(logger::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), outputFilePath.c_str());
    return error;
  }

  //success
  if (inputFilePath == outputFilePath)
    logger::log(logger::LOG_INFO, "Reorder of \"%s\" completed successfully.\n", inputFilePath.c_str());
  else
    logger::log(logger::LOG_INFO, "Reorder of \"%s\" into \"%s\" completed successfully.\n", inputFilePath.c_str(), outputFilePath.c_str());
	return APP_ERROR_SUCCESS;
}
