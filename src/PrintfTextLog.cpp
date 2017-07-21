#include "PrintfTextLog.h"

PrintfTextLog::PrintfTextLog() {}

PrintfTextLog::~PrintfTextLog() {}

void PrintfTextLog::Init() {}

void PrintfTextLog::addText(const std::string &newText, textLogColor color) {
  printf(newText.c_str());
  printf("\n");
}

void PrintfTextLog::Hide() {}

void PrintfTextLog::Show() {}
