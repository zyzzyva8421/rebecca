#include "project.h"
#include "information.h"
#include "casting.h"
#include "mold.h"
#include "entrance.h"
#include "simulation.h"
#include "computing.h"
#include "output.h"
#include <QFile>
#include <iostream>
#include <QXmlStreamWriter>
Project::Project(const wstring& _name) : Category(_name)
{
    information = new Information(L"Information");
    casting = new Casting(L"Casting");
    mold = new Mold(L"Mold");
    entrance = new Entrance(L"Entrance");
    simulation = new Simulation(L"Simulation");
    computing = new Computing(L"Computing");
    output = new Output(L"Output");
}

Project::~Project()
{
    if (information) delete information;
    information = NULL;
    if (casting) delete casting;
    casting = NULL;
    if (mold) delete mold;
    mold = NULL;
    if (entrance) delete entrance;
    entrance = NULL;
    if (simulation) delete simulation;
    simulation = NULL;
    if (computing) delete computing;
    computing = NULL;
    if (output) delete output;
    output = NULL;
}

void Project::writeValue(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Project");
    information->writeValue(writer);
    casting->writeValue(writer);
    mold->writeValue(writer);
    entrance->writeValue(writer);
    simulation->writeValue(writer);
    computing->writeValue(writer);
    output->writeValue(writer);
    writer.writeEndElement();
}

bool Project::writeConfigFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot write file "
                     << qPrintable(filename) << ": "
                        << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    writeValue(xmlWriter);
    xmlWriter.writeEndDocument();
    file.close();
    if (file.error()) {
        std::cerr << "Error: Cannot write file "
                     << qPrintable(filename) << ": "
                        << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    return true;
}

bool Project::loadConfigFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Can not read file" << qPrintable(filename)
                     << "; " << qPrintable(file.errorString())
                        << std::endl;

    }
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                     << "column " << errorColumn << ": "
                        << qPrintable(errorStr) << std::endl;
        return false;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "Project") {
        std::cerr << "Error: Not a project file" << std::endl;
        return false;
    }
    string name = filename.toStdString();
    loadValue(root);
    file.close();
    return true;
}

void Project::clearValue()
{
    if (information) information->clearValue();
    if (casting) casting->clearValue();
    if (mold) mold->clearValue();
    if (entrance) entrance->clearValue();
    if (simulation) simulation->clearValue();
    if (computing) computing->clearValue();
    if (output) output->clearValue();
}

void
Project::loadValue(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        string tagName = child.toElement().tagName().toStdString();
        if (tagName == "Information") {
            information->loadValue(child.toElement());
        } else if (tagName == "Casting") {
            casting->loadValue(child.toElement());
        } else if (tagName == "Mold") {
            mold->loadValue(child.toElement());
        } else if (tagName == "Entrance") {
            entrance->loadValue(child.toElement());
        } else if (tagName == "Simulation") {
            simulation->loadValue(child.toElement());
        } else if (tagName == "Computing") {
            computing->loadValue(child.toElement());
        } else if (tagName == "Output") {
            output->loadValue(child.toElement());
        }
        child = child.nextSibling();
    }
}

void Project::updateGui()
{
    information->updateGui();
    casting->updateGui();
    mold->updateGui();
    entrance->updateGui();
    simulation->updateGui();
    computing->updateGui();
    output->updateGui();
}

void Project::updateValue()
{
    information->updateValue();
    casting->updateValue();
    mold->updateValue();
    entrance->updateValue();
    simulation->updateValue();
    computing->updateValue();
    output->updateValue();
}
