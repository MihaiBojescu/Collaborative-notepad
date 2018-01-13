#include "include/textbox.h"
#include "include/functions.h"
#include "include/communicationthread.h"

#include <QJsonObject>
#include <QJsonDocument>

TextBox::TextBox(CommunicationThread *thread)
{
    oldText = this->toPlainText();
    this->thread = thread;
    this->fileName = "|NULL|";
    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(thread, SIGNAL(receivedMessage(QJsonObject)), this, SLOT(onJsonReceived(QJsonObject)));
}

TextBox::~TextBox()
{
    disconnect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    disconnect(thread, SIGNAL(receivedMessage(QJsonObject)), this, SLOT(onJsonReceived(QJsonObject)));
}

void TextBox::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void TextBox::setIgnoreNextEvent()
{
    this->ignoreNextEvent = true;
}

QString TextBox::getDifference(QString newString, QString originalString = "")
{
    return newString;
}

void TextBox::onTextChanged()
{
    if(!this->ignoreNextEvent && this->thread->getSocket() != -1 && this->fileName != "|NULL|")
    {
        QString diff = this->getDifference(this->toPlainText(), this->oldText);

        QJsonObject object;
        object["reason"] = "Edit file";
        object["filename"] = this->fileName;
        object["filecontent"] = diff;

        QJsonDocument document(object);
        QString result(document.toJson(QJsonDocument::Compact));

        sendString(thread->getSocket(), result);

    }
    this->ignoreNextEvent = false;
    this->oldText = this->toPlainText();
}

void TextBox::onJsonReceived(QJsonObject object)
{
    if(object.contains("reason"))
        if(object["reason"] == "Edit file")
            if(this->fileName == object["filename"].toString())
            {
                QString oldText = this->toPlainText();
                QString newText = object["filecontent"].toString();

                int diffX = newText.length() - oldText.length();
                int max = newText.length() < oldText.length() ? oldText.length() : newText.length();

                if(this->cursor().pos().x() + diffX >= 0 && this->cursor().pos().x() + diffX < max)
                    this->cursor().setPos(this->cursor().pos().x() + diffX, this->cursor().pos().y());
                else if(this->cursor().pos().x() + diffX < 0)
                    this->cursor().setPos(0, this->cursor().pos().y());
                else
                    this->cursor().setPos(max, this->cursor().pos().y());

                this->setIgnoreNextEvent();
                this->setText(newText);
            }
}

