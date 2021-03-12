#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QString>

class SearchResult
{
public:
    QString text;
    QString link;
    SearchResult(QString i_text, QString i_link);
};

#endif // SEARCHRESULT_H
