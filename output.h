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
    Output(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue();
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
