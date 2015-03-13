#ifndef CATEGORY_H
#define CATEGORY_H
#include <string>
#include <vector>
#include <QDomDocument>
#include <QXmlStreamWriter>

using namespace std;
class Category
{
public:
    Category(const wstring &name);
    virtual ~Category();

    virtual void loadValue(const QDomElement& element) = 0;
    virtual void writeValue(QXmlStreamWriter& writer) = 0;
    virtual void updateGui(void) = 0;
    virtual void updateValue(void) = 0;
    virtual void clearValue(void) = 0;

    void setName(const wstring& _name) {name = _name;}
    wstring getName(void) const {return name;}

    void* getUi();
private:
    wstring name;
};
#endif // CATEGORY_H
