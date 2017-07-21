#ifndef CONFIRMATIONPROMPT_H
#define CONFIRMATIONPROMPT_H

#include <CEGUI/CEGUI.h>

class ConfirmationPrompt {
public:
  ConfirmationPrompt();
  virtual ~ConfirmationPrompt();
  static void Initialize(CEGUI::Event::Subscriber yesSubscription);

protected:
private:
  //! Handle No and Close Button click
  bool Handle_No(CEGUI::EventArgs &e);
};

#endif // CONFIRMATIONPROMPT_H
