#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "include/communicationthread.h"

#include <QTextEdit>
#include <QJsonObject>

class TextBox : public QTextEdit
{
    Q_OBJECT

public:
    TextBox(CommunicationThread* thread);
    ~TextBox();
    void setFileName(QString fileName);
    void setIgnoreNextEvent();

private:
    QString getDifference(QString newString, QString originalString);

    QString oldText;
    QString fileName;
    bool ignoreNextEvent;
    CommunicationThread* thread;

public slots:
    void onTextChanged();
    void onJsonReceived(QJsonObject object);
};

#endif // TEXTBOX_H
