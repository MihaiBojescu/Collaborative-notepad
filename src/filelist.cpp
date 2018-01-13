#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <unistd.h>
#include "include/custombutton.h"
#include "include/functions.h"
#include "include/filelist.h"

FileList::FileList(CommunicationThread *thread)
{
    this->socket = thread->getSocket();
    this->layout = new QVBoxLayout();
    this->setLayout(this->layout);
    connect(thread, SIGNAL(receivedMessage(QJsonObject)), this, SLOT(onJsonReceived(QJsonObject)));

    QJsonObject object;
    object["reason"] = "Send all files";
    QJsonDocument document(object);
    QString result(document.toJson(QJsonDocument::Compact));

    ::sendString(this->socket, result);

    this->setWindowTitle("Open network file");
}

FileList::~FileList()
{
    for(auto currentLayout: this->layout->children())
    {
        for(auto currentElement: currentLayout->children())
             delete currentElement;
        delete currentLayout;
    }
    delete this->layout;
}

QString FileList::getOption()
{
    return this->filename;
}

void FileList::buttonHandler()
{
    CustomButton* button = (CustomButton*)sender();
    QString file = button->getLabelText();
    this->filename = file;
    this->done(1);
}

void FileList::onJsonReceived(QJsonObject object)
{
    if(object.contains("reason"))
    {
        if(object["reason"] == "Send all files")
        {
            if(object.contains(QString("files")))
            {
                QJsonArray responseArray = object["files"].toArray();

                if(object["files"].isArray() && object["files"].toArray().isEmpty())
                {
                    QLabel* newLabel = new QLabel("No files are currently open");
                    this->resize(150, 100);
                    this->layout->addWidget(newLabel);
                }
                else
                {
                    for(QJsonValueRef element: responseArray)
                    {
                        QString string = element.toString();
                        QHBoxLayout* newLayout = new QHBoxLayout();
                        QLabel* newLabel = new QLabel(string.mid(0, string.size() - 1));
                        this->layout->addLayout(newLayout);
                        newLayout->addWidget(newLabel);


                        QChar lastChar = string.at(string.size() - 1);
                        if(lastChar == '+')
                        {
                            CustomButton* button = new CustomButton(string.mid(0, string.size() - 1));
                            button->setText("Edit this");
                            newLayout->addWidget(button);
                            connect(button, SIGNAL(released()), this, SLOT(buttonHandler()));
                        }
                        else
                        {
                            QPushButton* button = new QPushButton("Can't edit");
                            button->setEnabled(false);
                            newLayout->addWidget(button);
                        }
                    }
                }
            }
        }
    }
}
