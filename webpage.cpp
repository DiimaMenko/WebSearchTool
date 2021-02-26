#include "webpage.h"
#include "webhelper.h"

WebPage::WebPage(QString searchPhrase, QString url)
{
    _searchPhrase = searchPhrase;
    _url = url;
    _fullHtml = WebHelper::DownloadHtml(url);
}

void WebPage::GetBodyFromHtml()
{
    int64_t startBodyIndex = _fullHtml.indexOf("<body");
    if(startBodyIndex < 0)
    {
        error = Error_No_Body;
        return;
    }

    startBodyIndex = _fullHtml.indexOf(">", startBodyIndex);
    if(startBodyIndex < 0)
    {
        error = Error_Parsing;
        return;
    }

    int64_t endBodyIndex = _fullHtml.indexOf("</body>");
    if(endBodyIndex < 0)
    {
        error = Error_No_Body;
        return;
    }

    this->startBodyIndex = startBodyIndex + 1;
    this->endBodyIndex = endBodyIndex;

    int64_t startIndex = startBodyIndex + 1;
    int64_t length = 1;
    int64_t endIndex;
    while(startIndex <= endBodyIndex)
    {
        qDebug() << startIndex << "\t" << endIndex;
        if(startIndex == _fullHtml.indexOf("<script", startIndex - 1))
        {
            startIndex = _fullHtml.indexOf("</script>", startIndex);
        }
        else if(startIndex == _fullHtml.indexOf("<style", startIndex - 1))
        {
            startIndex = _fullHtml.indexOf("</style>", startIndex);
        }

        endIndex = _fullHtml.indexOf("<", startIndex);

        length = endIndex - startIndex;
        _bodyText.append(_fullHtml.data() + startIndex, length);
        startIndex = _fullHtml.indexOf(">", endIndex) + 1;
    }
}

void WebPage::GetLinksFromBody()
{
    int64_t current_index = startBodyIndex;
    while(current_index < endBodyIndex)
    {

    }
}

void WebPage::GetBodyTextFromHtml()
{

}

QString WebPage::BodyText()
{
    return _bodyText;
}

QString WebPage::FullHtml()
{
    return _fullHtml;
}

QString WebPage::Url()
{
    return _url;
}

bool WebPage::IsPhraseFound()
{
    return _bodyText.indexOf(_searchPhrase, 0, Qt::CaseInsensitive) > -1;
}
