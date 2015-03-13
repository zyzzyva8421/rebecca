#ifndef INFORMATION_H
#define INFORMATION_H
#include "category.h"

class Information : public Category
{
public:
    Information(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue(void);

    void* getDialogUi();
    void updateDialogGui(void);
    void updateValueFromDialog(void);

    wstring getProjectPath(void) {return projectPath;}

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
