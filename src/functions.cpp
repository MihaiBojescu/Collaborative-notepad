#include "include/functions.h"
#include <QString>
#include <unistd.h>

QString readStringFromSocket(int& socket)
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
            close(socket);
            socket = -1;
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
