#ifndef TECHNIQUESELECT_H
#define TECHNIQUESELECT_H

#include <CEGUI/CEGUI.h>
#include "Character.h"
#include <functional>

class TechniqueSelect
{
public:
    TechniqueSelect() {}
    ~TechniqueSelect() {}

    void Cleanup();

    void Show();
    void Hide();

    void Initialize();

    void registerCharacter(Character* character);

    void registerActions(std::function<void(void)> cancelAction, std::function<void(void)> continueAction);
    //void registerActions(CEGUI::SubscriberSlot cancelAction, CEGUI::SubscriberSlot continueAction);
protected:
    CEGUI::Window* rootWindow;
    Character* m_character;

    bool SelectionChanged(const CEGUI::EventArgs& e);
    bool CleanupList(const CEGUI::EventArgs& e);

    void adjustWindowSize();
};

#endif
