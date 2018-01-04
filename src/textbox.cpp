#include <QQueue>
#include <QRegExp>
#include <QKeyEvent>
#include "include/textbox.h"

TextBox::TextBox()
{
    oldText = this->toPlainText();
    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

int TextBox::min(int a, int b, int c)
{
    return std::min(std::min(a, b), c);
}

int TextBox::editDistance(const QString a, const QString b)
{
    int matrix[a.size() + 1][b.size() + 1];

    for(int i = 0; i <= a.size(); ++i)
    {
        for(int j = 0; j <= b.size(); ++j)
        {
            if(i == 0)
                matrix[i][j] = j;

            else if(j == 0)
                matrix[i][j] = i;

            else if(a[i - 1] == b[j - 1])
                matrix[i][j] = matrix[i - 1][j - 1];

            else
                matrix[i][j] = 1 + min(matrix[i][j - 1],
                                       matrix[i - 1][j],
                                       matrix[i - 1][j - 1]);
        }
    }

    return matrix[a.size()][b.size()];
}

QStringList TextBox::buildWordList(const QString string)
{
    QStringList result;
    QRegExp expression("[a-zA-Z0-9]+[^a-zA-Z0-9]*");
    int position = 0;

    while ((position = expression.indexIn(string, position)) != -1)
    {
        result << expression.cap();
        position += expression.matchedLength();
    }

    return result;
}

QString TextBox::getDifference(QString a, QString b)
{

    if(a.size() == 0)
        return "- " + b;

    if(b.size() == 0)
        return "+ " + a;

    QString result = "";
    QStringList listOfStringsA = buildWordList(a);
    QStringList listOfStringsB = buildWordList(b);
    QQueue<word> queue;
    word minWord;
    int min;
    bool continueInside = true;

    for(int i = 0; i < listOfStringsA.size(); ++i)
    {
        QString stringA = listOfStringsA[i];

        queue.empty();
        min = stringA.size();
        continueInside = true;
        for(int j = 0; j < listOfStringsB.size() && continueInside; ++j)
        {
            QString stringB = listOfStringsB[j];

            if(stringA == stringB)
            {
                listOfStringsA.removeAt(listOfStringsA.indexOf(stringA));
                listOfStringsB.removeAt(listOfStringsB.indexOf(stringB));
                continueInside = false;

                // We removed the current element, so we need to reevaluate it
                --i;
            }
            else
            {
                word newWord;
                newWord.string = stringB;
                newWord.distance = this->editDistance(stringA, stringB);
                queue.push_back(newWord);
            }
        }
        if(continueInside)
        {
            for(word currentWord: queue)
            {
                if(currentWord.distance < min)
                {
                    min = currentWord.distance;
                    minWord = currentWord;
                }
            }
            if(min <= stringA.size())
            {
                listOfStringsA.removeAt(listOfStringsA.indexOf(stringA));
                listOfStringsB.removeAt(listOfStringsB.indexOf(minWord.string));
                if(minWord.string.size() > stringA.size())
                {
                    result += "+ " + minWord.string + "\n";
                    if(stringA.size() > 0)
                        result += "- " + stringA + "\n";
                }
                else
                {
                    result += "+ " + stringA + "\n";
                    if(minWord.string.size() > 0)
                        result += "- " + minWord.string + "\n";
                }
                --i;
            }
        }
    }

    for(QString string: listOfStringsA)
        result += "+ " + string + "\n";

    for(QString string: listOfStringsB)
        result += "- " + string + "\n";

    return result;
}

void TextBox::onTextChanged()
{
    QString diff = this->getDifference(this->toPlainText(), this->oldText);
    this->oldText = this->toPlainText();
}

