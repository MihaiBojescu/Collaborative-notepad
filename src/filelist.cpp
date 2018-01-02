#include <unistd.h>
#include "include/custombutton.h"
#include "include/functions.h"
#include "include/filelist.h"

FileList::FileList(int socket)
{
    this->socket = socket;
    this->layout = new QVBoxLayout();
    this->setLayout(this->layout);

    QString buffer;
    const char* sendString = "Send\0";

    if(::write(this->socket, sendString, ::strlen(sendString)) == -1)
    {
        ::perror("Write send message error.");
        ::exit(1);
    }

    do
    {
        buffer = readStringFromSocket(this->socket);

        if(buffer.size() > 0)
        {
            QChar lastChar = buffer.at(buffer.size() - 1);

            QHBoxLayout* newLayout = new QHBoxLayout();
            QLabel* newLabel = new QLabel(buffer.mid(0, buffer.size() - 1));
            this->layout->addLayout(newLayout);
            newLayout->addWidget(newLabel);

            if(lastChar == '+')
            {
                CustomButton* button = new CustomButton(buffer.mid(0, buffer.size() - 1));
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
    } while(buffer.size() != 0);

    this->exec();
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
