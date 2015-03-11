#ifndef PROJECT_H
#define PROJECT_H
#include "category.h"

class Project : public Category
{
public:
    Project(const string& _name);
    ~Project();

    void virtual loadValue(const QDomElement &element);
    void virtual updateGui(void);
    void virtual updateValue(void);

    void addCategory(Category *category) {categories.push_back(category);}
    Category *getCategory(const string& category);

private:
    vector<Category*> categories;
};

#endif // PROJECT_H
