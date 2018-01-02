#include "include/custombutton.h"

CustomButton::CustomButton(QString labelText)
{
    this->labelText = labelText;
}

QString CustomButton::getLabelText()
{
    return this->labelText;
}

void CustomButton::setLabelText(QString string)
{
    this->labelText = string;
}
