#include "webpage.h"
#include "webhelper.h"
#include <QTime>

WebPage::WebPage(QString searchPhrase, QString url)
{
    _errors = 0;
    _searchPhrase = searchPhrase;
    _url = url;

    auto startTime = QTime::currentTime();
    auto overalStartTime = startTime;
    _fullHtml = new QString(WebHelper::DownloadHtml(url));
    downloadingTime = startTime.msecsTo(QTime::currentTime());

    _searchPhraseFound = false;

    if(_fullHtml->isEmpty())
    {
        _errors |= Error_Downloading;
        return;
    }

    pageSize = _fullHtml->size();

    startTime = QTime::currentTime();
    GetTitleFromHtml();
    searchingTitleTime = startTime.msecsTo(QTime::currentTime());

    startTime = QTime::currentTime();
    GetBodyFromHtml();
    gettingBodyTime = startTime.msecsTo(QTime::currentTime());
    delete _fullHtml;

    startTime = QTime::currentTime();
    GetLinksFromBody();
    gettingLinksTime = startTime.msecsTo(QTime::currentTime());

    startTime = QTime::currentTime();
    GetTextFromBodyAndSearch();
    gettingTextFromBodyTime = startTime.msecsTo(QTime::currentTime());
    overalTime = overalStartTime.msecsTo(QTime::currentTime());

    /*if(overalTime - downloadingTime > 1000)
    {
        auto htmlFile = new QFile(Title().remove(QRegExp("[^a-zA-Z\\d\\s]")) + ".html");
        if(!htmlFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
        {
            return;
        }
        auto htmlFileStream = new QTextStream(htmlFile);
        *htmlFileStream << _url << "\n\n";
        *htmlFileStream << *_fullHtml;
        htmlFile->flush();
        htmlFile->close();
    }*/
}

void WebPage::GetTitleFromHtml()
{
    if(_errors != Success)
    {
        return;
    }

    int64_t startTitleIndex = _fullHtml->indexOf("<title");
    if(startTitleIndex < 0)
    {
        _errors |= Error_No_Title;
        return;
    }

    startTitleIndex = _fullHtml->indexOf(">", startTitleIndex + 1) + 1;
    if(startTitleIndex <= 0)
    {
        _errors |= Error_Parsing;
        return;
    }

    int64_t endTitleIndex = _fullHtml->indexOf("</title>");
    if(endTitleIndex < 0)
    {
        _errors |= Error_No_Title;
        return;
    }

    int64_t length = endTitleIndex - startTitleIndex;
    _title.append(_fullHtml->data() + startTitleIndex, length);
    _title = _title.remove("\n");
}

void WebPage::GetBodyFromHtml()
{
    if(_errors != Success)
    {
        return;
    }

    int64_t startBodyIndex = _fullHtml->indexOf("<body");
    if(startBodyIndex < 0)
    {
        _errors |= Error_No_Body;
        return;
    }

    startBodyIndex = _fullHtml->indexOf(">", startBodyIndex + 1) + 1;
    if(startBodyIndex <= 0)
    {
        _errors |= Error_Parsing;
        return;
    }

    int64_t endBodyIndex = _fullHtml->indexOf("</body>");
    if(endBodyIndex < 0)
    {
        _errors |= Error_No_Body;
        return;
    }

    int64_t length = endBodyIndex - startBodyIndex;
    _body.append(_fullHtml->data() + startBodyIndex, length);
}

void WebPage::GetTextFromBodyAndSearch()
{
    if(_errors != Success)
    {
        return;
    }

    QString bodyText = "";
    int64_t startIndex = 0;
    int64_t length = 1;
    int64_t endIndex = 1;

    if(_body.length() > 10000)  // If page is too big, just fast search through all body
    {
        _searchPhraseFound = _body.indexOf(_searchPhrase, 0, Qt::CaseInsensitive) > -1;
    }
    else    // Else, search through only visible text
    {
        while(startIndex <= _body.length())
        {
            if(startIndex == _body.indexOf("<script", startIndex - 1))
            {
                startIndex = _body.indexOf("</script>", startIndex);
            }
            else if(startIndex == _body.indexOf("<style", startIndex - 1))
            {
                startIndex = _body.indexOf("</style>", startIndex);
            }

            endIndex = _body.indexOf("<", startIndex + 1);
            if(endIndex < 0)
                break;

            length = endIndex - startIndex;
            bodyText.append(_body.data() + startIndex, length);
            startIndex = _body.indexOf(">", endIndex + 1);

            if(startIndex < endIndex)
                break;
        }

        _searchPhraseFound =  bodyText.indexOf(_searchPhrase, 0, Qt::CaseInsensitive) > -1;
    }
}

void WebPage::GetLinksFromBody()
{
    if(_errors != Success)
    {
        return;
    }

    int64_t startIndex = 0;
    int64_t length = 1;
    int64_t endIndex = 1;

    while(startIndex <= _body.length())
    {
        startIndex = _body.indexOf("\"http://", startIndex);
        if(startIndex < 0)
        {
            break;
        }

        endIndex = _body.indexOf("\"", startIndex + 1);
        if(endIndex <= 0)
            break;

        length = endIndex - startIndex - 1;
        QString link = "";
        link.append(_body.data() + startIndex + 1, length);

        link = link.mid(0, link.indexOf('#'));   // remove navigation inside page
        _bodyLinks.append(link);

        startIndex = endIndex + 1;
        if(startIndex <= 0)
            break;
    }
}

QString WebPage::Title() const
{
    return _title;
}

QList<QString> WebPage::GetLinks() const
{
    return _bodyLinks;
}

QString WebPage::Url() const
{
    return _url;
}

bool WebPage::IsPhraseFound() const
{
    return _searchPhraseFound;
}

int WebPage::GetErrorCode()
{
    return _errors;
}
