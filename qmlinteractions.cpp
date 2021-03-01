#include "qmlinteractions.h"
#include "webhelper.h"
#include "searchresult.h"

Q_INVOKABLE void QmlInteractions::runSearch(QString searchWord, QString startingUrl, int maximumScanUrls, int threadsCount)
{
    AddToLog("Starting search");
    stopPressed = false;
    searchProgress = 0.0;

    AddToLog("Clear previous results");
    searchResults.clear();
    links.clear();

    AddToLog("Maximum Url count = " + QString::number(maximumScanUrls));
    AddToLog("Threads count = " + QString::number(threadsCount));
    maximumScanUrlsCount = maximumScanUrls;
    this->threadsCount = threadsCount;

    AddToLog("Search phrase: " + searchWord);
    AddToLog("Starting Url: " + startingUrl);
    this->searchWord = searchWord;
    templink link;
    link.url = startingUrl;
    link.used = false;
    links.append(link);

//    AddToLog("Creating specified amount of threads.");
//    for(int i = 0; i < this->threadsCount; i++)
//    {
//        auto thread = new QThread(this);
//        _threads.append(thread);
//    }
    RunLoop();
}

void QmlInteractions::AddToLog(const QString &message)
{
    _log.AddMessage(message);
    emit logUpdated();
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
            AddResult(page.Title(), page.Url());
        }
        i++;
        searchProgress = double(i) / double(maximumScanUrlsCount);
        emit progressChanged();
    }

    searchProgress = 1.0;
    AddToLog("Search finished.");
    emit progressChanged();
    emit searchFinished();
}

void QmlInteractions::AddResult(QString title, QString url)
{
    lockResults.lockForWrite();
    auto result = new SearchResult(title, url);
    searchResults.append(*result);
    emit searchResultsChanged();
    lockResults.unlock();
}

void QmlInteractions::AddLinks(const QList<QString> &linkList)
{
    if(links.size() >= maximumScanUrlsCount)
    {
        AddToLog("Links count reached maximum, no more links will be added");
        return;
    }

    lockLinks.lockForWrite();

    templink link;
    for(int i = 0; i < linkList.size() && links.size() < maximumScanUrlsCount; i++)
    {
        link.url = linkList.at(i);
        link.used = false;

        if(!links.contains(link))
        {
            links.append(link);
        }
    }
    lockLinks.unlock();
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

Q_INVOKABLE QString QmlInteractions::getLastLogMessage() const
{
    return _log.GetLastMessage();
}

Q_INVOKABLE void QmlInteractions::stopSearch()
{
    stopPressed = true;
}
