#include "qmlinteractions.h"
#include "webhelper.h"
#include "searchresult.h"

Q_INVOKABLE void QmlInteractions::runSearch(QString searchWord, QString startingUrl, int maximumScanUrls)
{
    searchProgress = 0.0;
    maximumScanUrlsCount = maximumScanUrls;
    searchResults.clear();
    links.clear();

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
    while(i < links.size() && !stopPressed)
    {
        links[i].used = true;
        WebPage page(searchWord, links.at(i).url);
        AddLinks(page.GetLinks());

        if(page.IsPhraseFound())
        {
            auto result = new SearchResult(page.Title(), page.Url());
            searchResults.append(*result);
            emit searchResultsChanged();
        }
        i++;
        searchProgress = double(i) / double(maximumScanUrlsCount);
        emit progressChanged();
    }

    searchProgress = 1.0;
    emit progressChanged();
    emit searchFinished();
}

void QmlInteractions::AddLinks(const QList<QString> &linkList)
{
    for(int i = 0; i < linkList.size() && links.size() < maximumScanUrlsCount; i++)
    {
        templink link;
        link.url = linkList.at(i);
        link.used = false;

        if(!links.contains(link))
        {
            links.append(link);
        }
    }
}

Q_INVOKABLE double QmlInteractions::getSearchProgress() const
{
    return searchProgress;
}

Q_INVOKABLE QString QmlInteractions::getLastSearchResultTitle() const
{
    if(searchResults.size() > 0)
        return searchResults.last().text;
    else
        return "";
}

Q_INVOKABLE QString QmlInteractions::getLastSearchResultLink() const
{
    if(searchResults.size() > 0)
        return searchResults.last().link;
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

Q_INVOKABLE void QmlInteractions::stopSearch()
{
    stopPressed = true;
}
