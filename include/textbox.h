#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <QTextEdit>

class TextBox : public QTextEdit
{
    Q_OBJECT

public:
    TextBox();

private:
    int min(int a, int b, int c);
    int editDistance(const QString a, const QString b);
    QStringList buildWordList(const QString string);
    QString getDifferenceOld(QString a, QString b);
    QString getDifference(QString a, QString b);

    QString oldText;

    struct word
    {
        QString string;
        int distance;
    };

public slots:
    void onTextChanged();
};

#endif // TEXTBOX_H
