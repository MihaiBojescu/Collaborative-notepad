#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QString>

QString readStringFromSocket(int socket);
void sendString(int socket, QString string);
void sendEnd(int socket);

#endif // FUNCTIONS_H
