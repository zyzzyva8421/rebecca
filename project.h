#ifndef PROJECT_H
#define PROJECT_H
#include "category.h"
class Information;
class Casting;
class Mold;
class Entrance;
class Simulation;
class Computing;
class Output;

class Project : public Category
{
public:
    Project(const wstring& _name);
    ~Project();

    void virtual loadValue(const QDomElement &element);
    void virtual updateGui(void);
    void virtual updateValue(void);
    void virtual clearValue();
    bool loadConfigFile(const QString& filename);

    Mold *getMold(void) {return mold;}
    Information *getInformation(void) {return information;}
    Casting *getCasting(void) {return casting;}
    Entrance *getEntrance(void) {return entrance;}

private:
    Information *information;
    Casting *casting;
    Mold *mold;
    Entrance *entrance;
    Simulation *simulation;
    Computing *computing;
    Output *output;
};

#endif // PROJECT_H
