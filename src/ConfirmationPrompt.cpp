#include "ConfirmationPrompt.h"

void ConfirmationPrompt::Initialize(CEGUI::Event::Subscriber yesSubscription) {
  // ctor
  CEGUI::WindowManager *pWindowManager =
      CEGUI::WindowManager::getSingletonPtr();
  CEGUI::FrameWindow *m_Window =
      static_cast<CEGUI::FrameWindow *>(pWindowManager->createWindow(
          "TaharezLook/FrameWindow", "ConfirmationBox"));

  CEGUI::PushButton *yesButton =
      (CEGUI::PushButton *)pWindowManager->createWindow("TaharezLook/Button",
                                                        "Yes");
  yesButton->subscribeEvent(CEGUI::PushButton::EventClicked, yesSubscription);
  yesButton->setText("Yes");
  m_Window->addChild(yesButton);

  CEGUI::PushButton *noButton =
      (CEGUI::PushButton *)pWindowManager->createWindow("TaharezLook/Button",
                                                        "No");
  noButton->subscribeEvent(CEGUI::PushButton::EventClicked, yesSubscription);
  noButton->setPosition(
      CEGUI::UVector2(cegui_reldim(0.75f), cegui_reldim(0.5f)));
  noButton->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.2f)));
  m_Window->addChild(noButton);

  CEGUI::System::getSingleton()
      .getDefaultGUIContext()
      .getRootWindow()
      ->addChild(m_Window);
}

ConfirmationPrompt::ConfirmationPrompt() {}

ConfirmationPrompt::~ConfirmationPrompt() {
  // dtor
}
