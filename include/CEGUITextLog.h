#pragma once
#include "TextLog.h"
#include <CEGUI/CEGUI.h>
class CEGUITextLog :
    public TextLog
{
public:
    CEGUITextLog();
    ~CEGUITextLog();
    CEGUI::Listbox* textLog;

    // Inherited via TextLog
    virtual void Init() override;
    virtual void addText(const std::string & newText, textLogColor color = textLogColor(1, 1, 1, 1)) override;
    virtual void Hide() override;
    virtual void Show() override;
};

