#ifndef FILELIST_H
#define FILELIST_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class FileList : public QDialog
{
    Q_OBJECT
private:
    int socket;
    QVBoxLayout* layout;
    QString filename;

public:
    FileList(int socket);
    ~FileList();

    QString getOption();

signals:

public slots:
    void buttonHandler();
};

#endif // FILELIST_H
