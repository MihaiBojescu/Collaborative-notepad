#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <QTextEdit>

class TextBox : public QTextEdit
{
public slots:
    void keyReleaseEvent(QKeyEvent *e);
};

#endif // TEXTBOX_H
