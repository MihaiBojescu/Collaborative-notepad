#ifndef WINDOW_H
#define WINDOW_H
#include "include/textbox.h"
#include "include/communicationthread.h"
#include "include/filelist.h"

#include <QWidget>
#include <QBoxLayout>
#include <QTabWidget>
#include <QMenuBar>
#include <QAction>
#include <QJsonObject>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();

private:
    int socket;
    struct sockaddr_in sendAddress;

    QAction* openFileAction;
    QAction* openNetworkFileAction;
    QAction* saveFileAction;
    QAction* newFileAction;
    QAction* closeTabAction;

    FileList* list;

    QMenuBar* mainMenu;
    QMenu* fileMenu;

    QBoxLayout* layout;
    QTabWidget* tabWidget;
    CommunicationThread* commThread;

    void createSocket();
    void createActions();
    void createMenu();

    void sendNetworkFile(QString filename, TextBox* textBox);
    void recvNetworkFile(QString filename);

private slots:
    void openFile();
    void openNetworkFile();
    void saveFile();
    void newFile();
    void closeTab();

public slots:
    void onJsonReceived(QJsonObject object);
};

#endif // WINDOW_H
