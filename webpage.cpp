#include "webpage.h"
#include "webhelper.h"

WebPage::WebPage(QString searchPhrase, QString url)
{
    _errors = 0;
    _searchPhrase = searchPhrase;
    _url = url;
    _fullHtml = new QString(WebHelper::DownloadHtml(url));
    _searchPhraseFound = false;

    if(_fullHtml->isEmpty())
    {
        _errors |= Error_Downloading;
        return;
    }

    GetTitleFromHtml();
    GetBodyFromHtml();
    GetLinksFromBody();
    GetTextFromBodyAndSearch();
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
        _errors = Error_No_Title;
        return;
    }

    startTitleIndex = _fullHtml->indexOf(">", startTitleIndex) + 1;
    if(startTitleIndex <= 0)
    {
        _errors = Error_Parsing;
        return;
    }

    int64_t endTitleIndex = _fullHtml->indexOf("</title>");
    if(endTitleIndex < 0)
    {
        _errors = Error_No_Title;
        return;
    }

    int64_t length = endTitleIndex - startTitleIndex;
    _title.append(_fullHtml->data() + startTitleIndex, length);
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
        _errors = Error_No_Body;
        return;
    }

    startBodyIndex = _fullHtml->indexOf(">", startBodyIndex) + 1;
    if(startBodyIndex <= 0)
    {
        _errors = Error_Parsing;
        return;
    }

    int64_t endBodyIndex = _fullHtml->indexOf("</body>");
    if(endBodyIndex < 0)
    {
        _errors = Error_No_Body;
        return;
    }

    int64_t length = endBodyIndex - startBodyIndex;
    _body.append(_fullHtml->data() + startBodyIndex, length);
    delete _fullHtml;
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

        endIndex = _body.indexOf("<", startIndex);

        length = endIndex - startIndex;
        bodyText.append(_body.data() + startIndex, length);
        startIndex = _body.indexOf(">", endIndex) + 1;

        if(startIndex == 0)
            break;
    }

    _searchPhraseFound =  bodyText.indexOf(_searchPhrase, 0, Qt::CaseInsensitive) > -1;
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

        length = endIndex - startIndex - 1;
        QString link = "";
        link.append(_body.data() + startIndex + 1, length);

        _bodyLinks.append(link);

        startIndex = endIndex + 1;
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
    return this->_searchPhraseFound;
}
