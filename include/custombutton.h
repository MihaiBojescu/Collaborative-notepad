#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H
#include <QPushButton>

class CustomButton : public QPushButton
{
public:
    CustomButton(QString labelText);

    QString getLabelText();
    void setLabelText(QString string);

private:
    QString labelText;
};

#endif // CUSTOMBUTTON_H
