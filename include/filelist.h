#ifndef FILELIST_H
#define FILELIST_H

#include "include/communicationthread.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QJsonObject>
#include <QPushButton>

class FileList : public QDialog
{
    Q_OBJECT

private:
    int socket;
    QVBoxLayout* layout;
    QString filename;

public:
    FileList(CommunicationThread* thread);
    ~FileList();

    QString getOption();

signals:

public slots:
    void buttonHandler();
    void onJsonReceived(QJsonObject object);
};

#endif // FILELIST_H
