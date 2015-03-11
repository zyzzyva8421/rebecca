#ifndef OUTPUT_H
#define OUTPUT_H
#include "category.h"

class Output : public Category
{
public:
    enum OutputMethod {
        OutputIntervalTimeOn,
        OutputIntervalStepOn
    };
    Output(const string& name);
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);
private:
    OutputMethod outputMethod;
    double outputIntervalTimeOnConf;
    int outputIntervalStepOnConf;
    bool loggingCurrentStepOn;
    bool loggingCurrentTimeOn;
    bool loggingCurrentFillingRateOn;
    bool loggingCurrentTemperatureOn;
    int loggingFrequency;
    wstring outputComment;
};

#endif // OUTPUT_H
