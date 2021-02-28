#include "qmlinteractions.h"
#include "webhelper.h"

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
    while(i < links.size())
    {
        links[i].used = true;
        WebPage page(searchWord, links.at(i).url);
        AddLinks(page.GetLinks());

        if(page.IsPhraseFound())
        {
            searchResults.append(page.Url());
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
