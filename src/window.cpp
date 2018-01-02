#include <QIcon>
#include <QMenu>
#include <QKeySequence>
#include <QFileDialog>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "include/window.h"
#include "include/filelist.h"

Window::Window()
{
    this->mainMenu  = new QMenuBar(this);
    this->fileMenu  = new QMenu();
    this->layout    = new QBoxLayout(QBoxLayout::LeftToRight);
    this->tabWidget = new QTabWidget();
    this->textBox   = new TextBox();
    this->list      = NULL;

    this->createSocket();
    this->createActions();
    this->createMenu();

    this->layout->addWidget(this->tabWidget);
    this->layout->setMargin(0);
    this->layout->setMenuBar(this->mainMenu);
    this->tabWidget->addTab(this->textBox, tr("New tab"));

    this->setLayout(this->layout);
    this->setWindowIcon(QIcon(":/res/app.png"));
    this->resize(400, 300);
}

Window::~Window()
{
    delete this->textBox;
    delete this->tabWidget;
    delete this->layout;
    delete this->fileMenu;
    delete this->mainMenu;
    delete this->saveFileAction;
    delete this->openNetworkFileAction;
    delete this->openFileAction;

    ::close(this->socket);
}

void Window::createActions()
{
    this->openFileAction = new QAction(tr("Open file"), this);
    this->openFileAction->setShortcut(QKeySequence::Open);
    this->openFileAction->setStatusTip(tr("Open a file"));
    connect(openFileAction, &QAction::triggered, this, &Window::openFile);

    this->openNetworkFileAction = new QAction(tr("Open network file"), this);
    this->openNetworkFileAction->setShortcut(QKeySequence::fromString("ctrl+d"));
    this->openNetworkFileAction->setStatusTip(tr("Open a file from the network"));
    connect(openNetworkFileAction, &QAction::triggered, this, &Window::openNetworkFile);

    this->saveFileAction = new QAction(tr("Save file"), this);
    this->saveFileAction->setShortcut(QKeySequence::Save);
    this->saveFileAction->setStatusTip(tr("Save a file"));
    connect(this->saveFileAction, &QAction::triggered, this, &Window::saveFile);
}

void Window::createSocket()
{
    this->sendAddress.sin_family = AF_INET;
    this->sendAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    this->sendAddress.sin_port = htons(4000);


    if((this->socket = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation error");
        exit(1);
    }

    if(::connect(this->socket, (struct sockaddr*) &this->sendAddress, sizeof(this->sendAddress)) == -1)
    {
        perror("Connect error.");
        exit(1);
    }
}

void Window::createMenu()
{
    this->mainMenu->addMenu(this->fileMenu);
    this->fileMenu->setTitle("File");
    this->fileMenu->addAction(this->openFileAction);
    this->fileMenu->addAction(this->openNetworkFileAction);
    this->fileMenu->addAction(this->saveFileAction);
}


void Window::sendNetworkFile(QString filename)
{

}

void Window::recvNetworkFile()
{

}

void Window::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Text file (*.txt)"));
    if(fileName != "")
    {
        struct stat st;
        stat(fileName.toStdString().data(), &st);
        int fileSize = st.st_size;
        char* string = (char*)malloc(sizeof(char) * fileSize);

        int fileD = ::open(fileName.toStdString().data(), O_RDONLY);
        ::read(fileD, string, fileSize);
        ::close(fileD);
        string[fileSize] = '\0';

        this->textBox->setText(tr(string));
        this->tabWidget->setTabText(0, fileName);
        free(string);

        this->sendNetworkFile(fileName);
    }
}

void Window::openNetworkFile()
{
    if(this->list == NULL)
    {
        this->list = new FileList(this->socket);
        QString filename = this->list->getOption();
        delete this->list;
        this->list = NULL;
    }
}

void Window::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save current file"), "", tr("Text file (*.txt)"));
    if(fileName != "")
    {
        const char* string = this->textBox->toPlainText().toLatin1().data();
        fflush(stdout);

        int fileD = ::open(fileName.toStdString().data(), O_WRONLY);
        ::write(fileD, string, strlen(string));
        ::close(fileD);
    }
}
