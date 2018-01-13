#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H
#include <poll.h>
#include <QThread>
#include <QTabWidget>
#include <QJsonObject>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class CommunicationThread : public QThread
{
    Q_OBJECT

public:
    CommunicationThread();
    ~CommunicationThread();
    int getSocket();

protected:
    void run();

private:
    void attemptReconnect();

    int socket;
    sockaddr_in sendAddress;
    pollfd fds[1];


signals:
    void receivedMessage(QJsonObject message);
};

#endif // COMMUNICATIONTHREAD_H
