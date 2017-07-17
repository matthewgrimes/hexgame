#pragma once
#include "TextLog.h"
class PrintfTextLog : public TextLog {
public:
  PrintfTextLog();
  ~PrintfTextLog();

  virtual void Init();
  virtual void addText(const std::string &newText,
                       textLogColor color = textLogColor(1, 1, 1, 1));
  virtual void Hide();
  virtual void Show();
};
