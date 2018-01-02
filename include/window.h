#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include <QBoxLayout>
#include <QTabWidget>
#include <QMenuBar>
#include <QAction>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "include/textbox.h"
#include "include/filelist.h"

class Window : public QWidget
{
public:
    Window();
    ~Window();

private:
    int socket;
    struct sockaddr_in sendAddress;

    QAction* openFileAction;
    QAction* openNetworkFileAction;
    QAction* saveFileAction;

    FileList* list;

    QMenuBar* mainMenu;
    QMenu* fileMenu;

    QBoxLayout* layout;
    QTabWidget* tabWidget;
    TextBox* textBox;

    void createSocket();
    void createActions();
    void createMenu();

    void sendNetworkFile(QString filaname);
    void recvNetworkFile();

private slots:
    void openFile();
    void openNetworkFile();
    void saveFile();
};

#endif // WINDOW_H
