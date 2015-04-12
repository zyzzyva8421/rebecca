#ifndef ENTRANCE_H
#define ENTRANCE_H
#include "category.h"

class Entrance : public Category
{
public:
    enum Type {
        None,
        Inlet,
        Outlet
    };
    Entrance(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue(void);
private:
    Type boundaryRight;
    Type boundaryLeft;
    Type boundaryTop;
    Type boundaryBottom;
    Type boundaryFront;
    Type boundaryBack;
    wstring entranceComment;
};

#endif // ENTRANCE_H
