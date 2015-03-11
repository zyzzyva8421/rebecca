#ifndef INFORMATION_H
#define INFORMATION_H
#include "category.h"

class Information : public Category
{
public:
    Information(const string& name);
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);
private:
    wstring castingName;
    int castingNumber;
    wstring createdAuthor;
    string createdTime;
    wstring modifiedAuthor;
    string modifiedTime;
    bool basedProjectOn;
    wstring basedProjectPath;
    wstring projectLibaryPath;
    wstring projectPath;
    wstring projectComment;

};

#endif // INFORMATION_H
