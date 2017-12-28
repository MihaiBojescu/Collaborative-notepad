#include <QKeyEvent>
#include "include/textbox.h"

void TextBox::keyReleaseEvent(QKeyEvent *e)
{
    QTextEdit::keyReleaseEvent(e);
}
