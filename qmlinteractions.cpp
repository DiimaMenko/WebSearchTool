#include "qmlinteractions.h"
#include "webhelper.h"

Q_INVOKABLE void QmlInteractions::runSearch(QString searchWord, QString startingUrl, int maximumScanUrls)
{
    searchProgress = 0.0;
    maximumScanUrlsCount = maximumScanUrls;
    searchResults.clear();
    this->searchWord = searchWord;
    templink link;
    link.url = startingUrl;
    link.used = false;
    links.append(link);

    RunLoop();
}

void QmlInteractions::RunLoop()
{
    int i = 0;
    while(i < links.size())
    {
        WebPage page(searchWord, links.at(i).url);
        page.GetBodyFromHtml();
        page.GetLinksFromBody();
        AddLinks(page);

        if(page.IsPhraseFound())
        {
            searchResults.append(page.Url());
            emit searchResultsChanged();
        }
        i++;
        searchProgress = i / maximumScanUrlsCount;
    }
    searchProgress = 1.0;
    templink endlink;
    endlink.url = "Search Finished";
    links.append(endlink);
}

Q_INVOKABLE double QmlInteractions::getSearchProgress() const
{
    return searchProgress;
}

Q_INVOKABLE QString QmlInteractions::getLastSearchResults() const
{
    if(searchResults.size() > 0)
        return searchResults.last();
    else
        return "";
}

Q_INVOKABLE QString QmlInteractions::getExistingLinks() const
{
    QString linksTable = "";
    for(int i = 0; i < links.size(); i++)
    {
        auto link = links.at(i);
        linksTable.append(QString::number(i) + "\t" + (link.used ? "used\t" : "unused\t") + link.url + "\n");
    }
    return linksTable;
}

void QmlInteractions::AddLinks(const WebPage &page)
{
    for(int i = 0; i < page.GetLinks().size() && links.size() < maximumScanUrlsCount; i++)
    {
        templink link;
        link.url = page.GetLinks().at(i);
        link.used = false;

        if(!links.contains(link))
        {
            links.append(link);
        }
    }
}
