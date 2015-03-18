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
    enum LoggingMethod {
        LoggingIntervalTimeOn,
        LoggingIntervalStepOn
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
    LoggingMethod loggingMethod;
    double loggingIntervalTimeOnConf;
    int loggingIntervalStepOnConf;
    bool loggingCurrentStepOn;
    bool loggingCurrentTimeOn;
    bool loggingCurrentFillingRateOn;
    bool loggingCurrentTemperatureOn;
    int loggingFrequency;
    wstring outputComment;
};

#endif // OUTPUT_H
