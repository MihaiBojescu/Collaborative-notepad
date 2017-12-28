#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include <QBoxLayout>
#include <QTabWidget>
#include <QMenuBar>
#include <QAction>
#include "include/textbox.h"

class Window : public QWidget
{
public:
    Window();
    ~Window();

private:
    QAction* openFileAction;
    QAction* saveFileAction;

    QMenuBar* mainMenu;
    QMenu* fileMenu;

    QBoxLayout* layout;
    QTabWidget* tabWidget;
    TextBox* textBox;

    void createActions();
    void createMenu();

private slots:
    void openFile();
    void saveFile();
};

#endif // WINDOW_H
