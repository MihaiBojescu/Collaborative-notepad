#include "include/functions.h"
#include "include/window.h"
#include "include/filelist.h"

#include <QIcon>
#include <QMenu>
#include <QKeySequence>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


Window::Window()
{
    this->mainMenu  = new QMenuBar(this);
    this->fileMenu  = new QMenu();
    this->layout    = new QBoxLayout(QBoxLayout::LeftToRight);
    this->tabWidget = new QTabWidget();
    this->list      = NULL;

    this->createSocket();
    this->createActions();
    this->createMenu();

    connect(commThread, SIGNAL(receivedMessage(QJsonObject)), this, SLOT(onJsonReceived(QJsonObject)));

    this->layout->addWidget(this->tabWidget);
    this->layout->setMargin(0);
    this->layout->setMenuBar(this->mainMenu);

    this->setLayout(this->layout);
    this->setWindowIcon(QIcon(":/res/app.png"));
    this->setWindowTitle("Collaborative notepad");
    this->resize(640, 480);
}

Window::~Window()
{
    for(auto widget: this->tabWidget->children())
        delete widget;
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
    this->openNetworkFileAction->setShortcut(QKeySequence::fromString("ctrl+u"));
    this->openNetworkFileAction->setStatusTip(tr("Open a file from the network"));
    connect(openNetworkFileAction, &QAction::triggered, this, &Window::openNetworkFile);

    this->saveFileAction = new QAction(tr("Save file"), this);
    this->saveFileAction->setShortcut(QKeySequence::Save);
    this->saveFileAction->setStatusTip(tr("Save a file"));
    connect(this->saveFileAction, &QAction::triggered, this, &Window::saveFile);

    this->newFileAction = new QAction(tr("New file"), this);
    this->newFileAction->setShortcut(QKeySequence::New);
    this->newFileAction->setStatusTip(tr("Create a file"));
    connect(this->newFileAction, &QAction::triggered, this, &Window::newFile);

    this->closeTabAction = new QAction(tr("Close tab"), this);
    this->closeTabAction->setShortcut(QKeySequence::fromString("ctrl+w"));
    this->closeTabAction->setStatusTip(tr("Close current tab"));
    connect(this->closeTabAction, &QAction::triggered, this, &Window::closeTab);
}

void Window::createSocket()
{
    this->commThread = new CommunicationThread();
    this->commThread->start();
}

void Window::createMenu()
{
    this->mainMenu->addMenu(this->fileMenu);
    this->fileMenu->setTitle("File");
    this->fileMenu->addAction(this->openFileAction);
    this->fileMenu->addAction(this->openNetworkFileAction);
    this->fileMenu->addAction(this->saveFileAction);
    this->fileMenu->addAction(this->newFileAction);
    this->fileMenu->addAction(this->closeTabAction);
}


void Window::sendNetworkFile(QString filename, TextBox* textBox)
{
    QJsonObject object;
    object["reason"] = "Sending file";
    object["filename"] = filename;
    object["filecontent"] = textBox->toPlainText();

    QJsonDocument document(object);
    QString sendData = document.toJson(QJsonDocument::Compact);

    sendString(this->commThread->getSocket(), sendData);
}

void Window::recvNetworkFile(QString filename)
{
    QJsonObject object;
    object["reason"] = "Open file";
    object["filename"] = filename;

    QJsonDocument document(object);
    QString sendData = document.toJson(QJsonDocument::Compact);

    sendString(this->commThread->getSocket(), sendData);
}

void Window::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Text file (*.txt)"));
    if(fileName != "")
    {
        std::string fileNameSTD = fileName.toStdString();
        struct stat st;
        stat(fileNameSTD.data(), &st);
        int fileSize = st.st_size;
        char* string = (char*)malloc(sizeof(char) * fileSize);

        int fileD = ::open(fileNameSTD.data(), O_RDONLY);
        ::read(fileD, string, fileSize);
        ::close(fileD);
        string[fileSize] = '\0';

        size_t i = fileNameSTD.rfind("/", fileNameSTD.length());
        if(i != std::string::npos)
            fileName = fileNameSTD.substr(i + 1, fileNameSTD.length() - 1).c_str();

        TextBox* newTextBox = new TextBox(this->commThread);
        newTextBox->setIgnoreNextEvent();
        newTextBox->setFileName(fileName);
        newTextBox->setText(tr(string));
        int newTab = this->tabWidget->addTab(newTextBox, fileName);
        this->tabWidget->setCurrentIndex(newTab);
        free(string);

        this->sendNetworkFile(fileName, newTextBox);
    }
}

void Window::openNetworkFile()
{
    if(this->list == NULL)
    {
        this->list = new FileList(this->commThread);
        this->list->exec();

        QString fileName = this->list->getOption();

        if(fileName != "")
        {
            recvNetworkFile(fileName);
        }

        delete this->list;
        this->list = NULL;
    }
}

void Window::newFile()
{
    TextBox* newTextBox = new TextBox(this->commThread);
    int tabID = this->tabWidget->addTab(newTextBox, "Untitled");
    this->tabWidget->setCurrentIndex(tabID);
    newTextBox->setFocus();
}

void Window::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save current file"),
                                                    this->tabWidget->tabText(this->tabWidget->currentIndex()).mid(1, this->tabWidget->tabText(this->tabWidget->currentIndex()).length()),
                                                    tr("Any file (*.*)"));

    TextBox* textBox = (TextBox*) this->tabWidget->currentWidget();

    std::string filenameSTD = filename.toStdString();
    size_t i = filenameSTD.rfind("/", filenameSTD.length());
    if(i != std::string::npos)
        filename = filenameSTD.substr(i + 1, filenameSTD.length() - 1).c_str();

    textBox->setFileName(filename);
    this->tabWidget->setTabText(this->tabWidget->currentIndex(), filename);

    if(filename != "")
    {
        const char* string = textBox->toPlainText().toLatin1().data();

        int fileD = ::open(filename.toStdString().data(), O_WRONLY | O_CREAT | O_TRUNC);
        ::write(fileD, string, strlen(string));
        ::close(fileD);
    }
}

void Window::closeTab()
{
    TextBox* reference = (TextBox*) this->tabWidget->currentWidget();
    if(reference != NULL)
    {
        reference->disconnectEvents();
        if(reference->getFileName() != "|NULL|")
        {
            QJsonObject object;
            object["reason"] = "Disconnect";
            object["filename"] = reference->getFileName();

            QJsonDocument document(object);
            QString sendData = document.toJson(QJsonDocument::Compact);
            sendString(this->commThread->getSocket(), sendData);
        }
        delete reference;
    }
    this->tabWidget->removeTab(this->tabWidget->currentIndex());
}

void Window::onJsonReceived(QJsonObject object)
{
    qDebug() << object["reason"];
    if(object.contains("reason"))
        if(object["reason"] == "Open file")
            if(object["filename"].toString() != "|NULL|" && object["filecontent"].toString() != "|NULL|")
            {
                TextBox* newTextBox = new TextBox(this->commThread);
                newTextBox->setFileName(object["filename"].toString());
                newTextBox->setIgnoreNextEvent();
                int newTab = this->tabWidget->addTab(newTextBox, object["filename"].toString());
                this->tabWidget->setCurrentIndex(newTab);

                newTextBox->setText(object["filecontent"].toString());
            }
}
