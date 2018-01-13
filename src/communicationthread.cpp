#include "include/communicationthread.h"
#include "include/functions.h"
#include "include/textbox.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

CommunicationThread::CommunicationThread()
{
    this->sendAddress.sin_family = AF_INET;
    this->sendAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    this->sendAddress.sin_port = htons(4000);

    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    while(this->socket == -1)
    {
        perror("Socket creation error.");
        printf("Retrying...");
        fflush(stdout);
        this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
        this->sleep(1);
    }



    int connectStatus = ::connect(this->socket, (struct sockaddr*) &this->sendAddress, sizeof(this->sendAddress));
    while(connectStatus == -1)
    {
        perror("Connect error.");
        printf("Retrying...");
        fflush(stdout);
        connectStatus = ::connect(this->socket, (struct sockaddr*) &this->sendAddress, sizeof(this->sendAddress));
        this->sleep(1);
    }

    this->fds[0].fd = this->socket;
    this->fds[0].events = POLLIN;
}

CommunicationThread::~CommunicationThread()
{
    ::close(this->socket);
}

int CommunicationThread::getSocket()
{
    return this->socket;
}

void CommunicationThread::run()
{
    int maxFd;
    QString response;
    QJsonDocument responseDocument;
    QJsonObject responseObject;

    while(1)
    {
        maxFd = ::poll(this->fds, 1, -1);

        for(int i = 0; i < maxFd; i++)
            if(this->fds[i].revents == POLLIN && this->fds[i].fd == this->socket)
            {
                do
                {
                    response = readStringFromSocket(this->socket);
                    if(socket == -1)
                        attemptReconnect();
                    else
                    {
                        responseDocument = QJsonDocument::fromJson(response.toUtf8());
                        responseObject = responseDocument.object();
                    }
                } while(socket == -1);

                emit this->receivedMessage(responseObject);
            }
    }
}

void CommunicationThread::attemptReconnect()
{
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    while(this->socket == -1)
    {
        perror("Socket creation error.");
        printf("Retrying...");
        fflush(stdout);
        this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
        this->sleep(1);
    }

    int connectStatus = ::connect(this->socket, (struct sockaddr*) &this->sendAddress, sizeof(this->sendAddress));
    while(connectStatus == -1)
    {
        perror("Connect error.");
        printf("Retrying...");
        fflush(stdout);
        connectStatus = ::connect(this->socket, (struct sockaddr*) &this->sendAddress, sizeof(this->sendAddress));
        this->sleep(1);
    }
}
