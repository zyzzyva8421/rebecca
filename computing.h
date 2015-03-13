#ifndef COMPUTING_H
#define COMPUTING_H
#include "category.h"

class Computing : public Category
{
public:
    enum TimeStepMethod {
        TimeStepAdaptedOn,
        TimeStepFixedOn
    };
    enum TerminationCondition {
        TerminationEndTimeOn,
        TerminationFillingRateOn,
        TerminationMaximumStepOn
    };
    enum ParallelMethod {
        ParallelAdaptedOn,
        ParallelFixedOn
    };
    Computing(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue(void);
private:
    TimeStepMethod timeStepMethod;
    double timeStepFixedOnConf;
    double iterationMaximumRelativeError;
    int iterationMaximumStepCount;
    int iterationMinimumStepCount;
    TerminationCondition terminationCondition;
    double terminationEndTimeOnConf;
    double terminationFillingRateOnConf;
    int terminationMaximumStepOnConf;
    ParallelMethod parallelMethod;
    int parallelFixedOnConf;
    wstring computingComment;
};

#endif // COMPUTING_H
