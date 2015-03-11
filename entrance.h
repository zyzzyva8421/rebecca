#ifndef ENTRANCE_H
#define ENTRANCE_H
#include "category.h"

class Entrance : public Category
{
public:
    Entrance(const string& name);
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);
private:
    vector< vector<double> > outletCoordinates;
    vector< vector<double> > inletCoordinates;
    wstring entranceComment;
};

#endif // ENTRANCE_H
