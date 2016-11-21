#ifndef TEXTLOG_H
#define TEXTLOG_H

#include <string>

struct textLogColor
{
    textLogColor(float r, float g, float b, float alpha = 1.f) : R(r), G(g), B(b), Alpha(alpha)
    {

    }
    float R;
    float G;
    float B;
    float Alpha;
};

class TextLog
{

public:
    TextLog() {}
    virtual ~TextLog() {}

    virtual void Init() = 0;
    virtual void addText(const std::string& newText, textLogColor color = textLogColor(1,1,1,1)) = 0;
    virtual void Hide() = 0;
    virtual void Show() = 0;

};

class NullTextLog : public TextLog
{
    virtual void Init() {}
    virtual void addText(const std::string& newText, textLogColor color = textLogColor(1, 1, 1, 1)) {}
    virtual void Hide() {}
    virtual void Show() {}
};

#endif
