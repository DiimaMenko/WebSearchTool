#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <QString>
#include <QList>

#define Success 0x0000
#define Error_No_Body 0x0001
#define Error_No_Links 0x0002
#define Error_Parsing 0x0004

class WebPage
{
    QString _searchPhrase;
    QString _url;
    QString _fullHtml;
    QString _bodyText;
    QList<QString> _bodyLinks;

    int64_t startBodyIndex;
    int64_t endBodyIndex;

    int error;

public:
    WebPage(QString searchPhrase, QString url);

    void GetBodyFromHtml();
    void GetLinksFromBody();
    void GetBodyTextFromHtml();

    QString BodyText();
    QString FullHtml();
    QString Url();
    bool IsPhraseFound();
};

#endif // WEBPAGE_H
