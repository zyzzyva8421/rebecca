#include "project.h"
#include "information.h"
#include "casting.h"
#include "mold.h"
#include "entrance.h"
#include "simulation.h"
#include "computing.h"
#include "output.h"
Project::Project(const string& _name) : Category(_name)
{
}

Project::~Project()
{
    for (vector<Category*>::iterator it = categories.begin(); it != categories.end(); it++) {
        Category *category = (*it);
        delete category;
        category = NULL;
    }
    categories.clear();
}

void
Project::loadValue(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        Category *category = NULL;
        string tagName = child.toElement().tagName().toStdString();
        if (tagName == "Information") {
            category = new Information("Information");
        } else if (tagName == "Casting") {
            category = new Casting("Casting");
        } else if (tagName == "Mold") {
            category = new Mold("Mold");
        } else if (tagName == "Entrance") {
            category = new Entrance("Entrance");
        } else if (tagName == "Simulation") {
            category = new Simulation("Simulation");
        } else if (tagName == "Computing") {
            category = new Computing("Computing");
        } else if (tagName == "Output") {
            category = new Output("Output");
        }
        if (category) {
            addCategory(category);
            category->loadValue(child.toElement());
        }
        child = child.nextSibling();
    }
}

void Project::updateGui()
{
    for (vector<Category*>::iterator cat_it = categories.begin(); cat_it != categories.end(); cat_it++) {
        Category *cat = (*cat_it);
        if (cat) cat->updateGui();
    }
}

void Project::updateValue()
{
    for (vector<Category*>::iterator cat_it = categories.begin(); cat_it != categories.end(); cat_it++) {
        Category *cat = (*cat_it);
        if (cat) cat->updateValue();
    }
}

Category *Project::getCategory(const string &name)
{
    vector<Category*>::iterator it;
    Category *category = NULL;
    for (it = categories.begin(); it != categories.end(); it++) {
        category = (*it);
        if (category && category->getName() == name) {
            return category;
        }
    }
    return NULL;
}
