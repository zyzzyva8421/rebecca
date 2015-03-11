#ifndef CATEGORY_H
#define CATEGORY_H
#include <string>
#include <vector>
#include <QDomDocument>

using namespace std;
class Category
{
public:
    Category(const string &name);
    virtual ~Category();

    virtual void loadValue(const QDomElement& element) = 0;
    virtual void updateGui(void) = 0;
    virtual void updateValue(void) = 0;

    void setName(const string& _name) {name = _name;}
    string getName(void) const {return name;}

    void* getUi();
private:
    string name;
};
#endif // CATEGORY_H
