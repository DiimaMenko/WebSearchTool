#include "qmlinteractions.h"
#include "webhelper.h"
#include "searchresult.h"
#include <QtConcurrent/QtConcurrent>

Q_INVOKABLE void QmlInteractions::runSearch(QString searchWord, QString startingUrl, int maximumScanUrls, int threadsCount)
{
    AddToLog("Starting search");
    _stopPressed = false;
    _searchProgress = 0.0;
    _currentResultIterator = 0;
    _nextLink = 0;
    _finishedThreadsCount = 0;

    if(maximumScanUrls < threadsCount)
    {
        AddToLog("Setup have maximum scan urls coun bigger than thread count. Reducing thread count to " + QString::number(threadsCount));
        threadsCount = maximumScanUrls;
    }

    AddToLog("Clear previous results");
    _searchResults.clear();
    _links.clear();

    AddToLog("Maximum Url count = " + QString::number(maximumScanUrls));
    AddToLog("Threads count = " + QString::number(threadsCount));
    _maximumScanUrlsCount = maximumScanUrls;
    _threadsCount = threadsCount;

    AddToLog("Search phrase: " + searchWord);
    AddToLog("Starting Url: " + startingUrl);
    _searchWord = searchWord;
    templink link;
    link.url = startingUrl;
    link.used = false;
    _links.append(link);

    AddToLog("Creating specified amount of threads.");
    for(int i = 0; i < _threadsCount; i++)
    {
        SearchWorker *worker = new SearchWorker(this, searchWord);
        _threads.append(worker);
        connect(worker, &SearchWorker::ThreadFinishedWork, this, &QmlInteractions::ThreadFinishedWork);
        _threads[i]->start();
    }
}

void QmlInteractions::ThreadFinishedWork(QString threadId)
{
    AddToLog("Thread " + threadId + " finished work.");
    _lockFinishedThreadsCount.lockForWrite();
    _finishedThreadsCount++;
    _lockFinishedThreadsCount.unlock();

    if(_finishedThreadsCount == _threadsCount)
    {
        emit searchFinished();
    }
}

void QmlInteractions::AddToLog(const QString &message)
{
    _log.AddMessage(message);
    //emit logUpdated();
}

QString QmlInteractions::GetNextLink()
{
    QString returnLink;

    while(_nextLink >= _links.size())
    {
        if(_nextLink >= _maximumScanUrlsCount)
        {
            AddToLog("Looks like no more links can be scanned current: "
                     + QString::number(_nextLink)
                     + " already present in list: "
                     + QString::number(_links.size()));
            return "";
        }
        AddToLog("Waiting for adding more links current: "
                 + QString::number(_nextLink)
                 + " already present in list: "
                 + QString::number(_links.size()));
        QThread::msleep(100);
    }

    _lockNextLinkIterator.lockForWrite();

    returnLink = _links[_nextLink].url;
    _links[_nextLink].used = true;
    _nextLink++;

    _searchProgress = double(_nextLink) / double(_maximumScanUrlsCount);

    _lockNextLinkIterator.unlock();

    emit progressChanged();

    return returnLink;
}

void QmlInteractions::AddResult(QString title, QString url)
{
    _lockResults.lockForWrite();
    auto result = new SearchResult(title, url);
    _searchResults.append(*result);
    emit searchResultsChanged();
    _lockResults.unlock();
}

void QmlInteractions::AddLinks(const QList<QString> &linkList)
{
    AddToLog("Adding found links to storage");
    if(_links.size() >= _maximumScanUrlsCount)
    {
        AddToLog("Links count reached maximum, no more links will be added");
        return;
    }

    _lockLinks.lockForWrite();

    templink link;
    for(int i = 0; i < linkList.size() && _links.size() < _maximumScanUrlsCount; i++)
    {
        link.url = linkList.at(i);
        link.used = false;

        if(!_links.contains(link))
        {
            _links.append(link);
        }
    }
    _lockLinks.unlock();
}

Q_INVOKABLE double QmlInteractions::getSearchProgress() const
{
    return _searchProgress;
}

Q_INVOKABLE QString QmlInteractions::getLastSearchResultTitle()
{
    if(_searchResults.size() > 0)
    {
        return _searchResults.at(_currentResultIterator).text;
    }
    else
        return "";
}

Q_INVOKABLE QString QmlInteractions::getLastSearchResultLink() const
{
    if(_searchResults.size() > 0)
        return _searchResults.at(_currentResultIterator).link;
    else
        return "";
}

Q_INVOKABLE void QmlInteractions::getNextResult()
{
    _currentResultIterator++;
}

Q_INVOKABLE QString QmlInteractions::getLastLogMessage()
{
    return _log.GetLastMessage();
}

Q_INVOKABLE void QmlInteractions::stopSearch()
{
    _stopPressed = true;
}

