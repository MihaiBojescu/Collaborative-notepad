#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QString>

QString readStringFromSocket(int& socket);
void sendString(int socket, QString string);

#endif // FUNCTIONS_H
