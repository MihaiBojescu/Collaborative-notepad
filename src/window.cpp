#include <QIcon>
#include <QMenu>
#include <QKeySequence>
#include "include/window.h"

Window::Window()
{
    this->mainMenu  = new QMenuBar(this);
    this->fileMenu  = new QMenu();
    this->layout    = new QBoxLayout(QBoxLayout::LeftToRight);
    this->tabWidget = new QTabWidget();
    this->textBox   = new TextBox();

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
    delete this->openFileAction;
}

void Window::createActions()
{
    this->openFileAction = new QAction(tr("Open file"), this);
    this->openFileAction->setShortcut(QKeySequence::Open);
    this->openFileAction->setStatusTip(tr("Open a file"));
    connect(openFileAction, &QAction::triggered, this, &Window::openFile);

    this->saveFileAction = new QAction(tr("Save file"), this);
    this->saveFileAction->setShortcut(QKeySequence::Save);
    this->saveFileAction->setStatusTip(tr("Save a file"));
    connect(this->saveFileAction, &QAction::triggered, this, &Window::saveFile);
}

void Window::createMenu()
{
    this->mainMenu->addMenu(this->fileMenu);
    this->fileMenu->setTitle("File");
    this->fileMenu->addAction(this->openFileAction);
    this->fileMenu->addAction(this->saveFileAction);
}

void Window::openFile()
{
}

void Window::saveFile()
{
}
