#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <QString>
#include <QList>

#define Success 0x0000
#define Error_Downloading 0x0001
#define Error_No_Body 0x0002
#define Error_Parsing 0x0004
#define Error_No_Title 0x008

class WebPage
{
    QString _searchPhrase;
    QString _url;
    QString _title;
    QString *_fullHtml; //is deleting right after extracting body. This was decided to realize as too many useles in our case data is stored before and after body.
    QString _body;
    QList<QString> _bodyLinks;

    bool _searchPhraseFound;

    int _errors;


public:
    WebPage(QString searchPhrase, QString url);

    void GetBodyFromHtml();
    void GetTextFromBodyAndSearch();
    void GetLinksFromBody();
    void GetTitleFromHtml();
    int GetErrorCode();

    QString Url() const;
    QString Title() const;
    QList<QString> GetLinks() const;
    bool IsPhraseFound() const;

    int pageSize;
    int downloadingTime;
    int searchingTitleTime;
    int gettingBodyTime;
    int gettingLinksTime;
    int gettingTextFromBodyTime;
    int overalTime;
};

#endif // WEBPAGE_H
