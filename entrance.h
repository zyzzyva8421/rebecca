#ifndef ENTRANCE_H
#define ENTRANCE_H
#include "category.h"

class Entrance : public Category
{
public:
    Entrance(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue(void);
    vector< vector<double> >& getOutletCoordinates(void) {return outletCoordinates;}
    vector< vector<double> >& getInletCoordinates(void) {return inletCoordinates;}
private:
    vector< vector<double> > outletCoordinates;
    vector< vector<double> > inletCoordinates;
    wstring entranceComment;
};

#endif // ENTRANCE_H
