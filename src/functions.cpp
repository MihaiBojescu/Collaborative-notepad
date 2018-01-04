#include <QString>
#include <unistd.h>
#include "include/functions.h"

QString readStringFromSocket(int socket)
{
    QString buffer = "";
    char currentChar;
    int bytesWritten;

    do
    {
        if((bytesWritten = ::read(socket, &currentChar, 1)) == -1)
        {
            perror("Read error.");
            exit(1);
        }
        else if(bytesWritten == 0)
        {
            exit(1);
        }
        else
        {
            if(currentChar != 0)
                buffer += currentChar;
        }
    } while(currentChar != 0);

    return buffer;
}

void sendString(int socket, QString string)
{
    if(write(socket, string.toStdString().c_str(), string.size() + 1) == -1)
    {
        perror("Write error.");
        exit(1);
    }
}

void sendEnd(int socket)
{
    if(write(socket, "\0", 1) == -1)
    {
        perror("Write error.");
        exit(1);
    }
}
