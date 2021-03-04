#include "qmlinteractions.h"
#include "webhelper.h"
#include "searchresult.h"
#include <QtConcurrent/QtConcurrent>

Q_INVOKABLE void QmlInteractions::runSearch(QString searchWord, QString startingUrl, int maximumScanUrls, int threadsCount)
{
    AddToLog(message, "Search started.");
    _pausePressed = false;
    _stopPressed = false;
    _searchProgress = 0.0;
    _currentResultIterator = 0;
    _nextLink = 0;
    _finishedThreadsCount = 0;
    threadsWaiting = 0;

    if(maximumScanUrls < threadsCount)
    {
        AddToLog(message, "Setup have maximum scan urls coun bigger than thread count. Reducing thread count to " + QString::number(threadsCount));
        threadsCount = maximumScanUrls;
    }

    AddToLog(message, "Clear previous results");
    _searchResults.clear();
    _links.clear();

    AddToLog(message, "Maximum Url count = " + QString::number(maximumScanUrls));
    AddToLog(message, "Threads count = " + QString::number(threadsCount));
    _maximumScanUrlsCount = maximumScanUrls;
    _threadsCount = threadsCount;

    AddToLog(message, "Search phrase: " + searchWord);
    AddToLog(message, "Starting Url: " + startingUrl);
    _searchWord = searchWord;
    templink link;
    link.url = startingUrl;
    link.used = false;
    _links.append(link);

    AddToLog(message, "Creating specified amount of threads.");
    //TODO think how we can create threads only when enough links present
    //     and break threads creation if all alive threads finished work and no unused links left.
    for(int i = 0; i < _threadsCount; i++)
    {
        SearchWorker *worker = new SearchWorker(this, searchWord);
        _threads.append(worker);
        connect(worker, &SearchWorker::ThreadFinishedWork, this, &QmlInteractions::ThreadFinishedWork);
        connect(worker, &SearchWorker::finished, worker, &SearchWorker::deleteLater);
        _threads[i]->start();
    }
}

void QmlInteractions::ThreadFinishedWork(QString threadId)
{
    AddToLog(message, "Thread " + threadId + " finished work.");
    _lockFinishedThreadsCount.lockForWrite();
    _finishedThreadsCount++;
    _lockFinishedThreadsCount.unlock();

    if(_finishedThreadsCount == _threadsCount)
    {
        AddToLog(message, "Search finished.");
        _threads.clear();
        _searchProgress = 1.0;
        emit searchFinished();
    }
    else
    {
        AddToLog(message, " " + QString::number(_finishedThreadsCount) + " threads already finished work.");
    }
}

void QmlInteractions::AddToLog(MessageType messageType, const QString &message)
{
    _log.AddMessage(messageType, message);
    emit logUpdated();
}

QString QmlInteractions::GetNextLink()
{
    QString returnLink;

    _waitingThreadsCountLock.lockForWrite();
    threadsWaiting++;
    _waitingThreadsCountLock.unlock();

    int i = 0;

    _lockNextLinkIterator.lockForWrite();
    while(_nextLink >= _links.size())
    {
        if(i++ > 999)
        {
            AddToLog(message, "New links was not added too long time. Exiting this thread now.");
            _lockNextLinkIterator.unlock();
            return "";
        }

        if(_nextLink >= _maximumScanUrlsCount)
        {
            AddToLog(message, "Looks like no more links can be scanned.");
            _lockNextLinkIterator.unlock();
            return "";
        }

        if(threadsWaiting >= _threadsCount - _finishedThreadsCount)
        {
            AddToLog(message, "Looks like all threads stucked with no links to search. Exiting now.");
            _lockNextLinkIterator.unlock();
            return "";
        }

        AddToLog(message, "Waiting for adding more links current: "
                 + QString::number(_nextLink)
                 + " already present in list: "
                 + QString::number(_links.size()));

        QThread::msleep(100);
    }

    _waitingThreadsCountLock.lockForWrite();
    threadsWaiting--;
    _waitingThreadsCountLock.unlock();

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
    AddToLog(message, "Adding " + QString::number(linkList.size()) + " found links to storage");
    if(_links.size() >= _maximumScanUrlsCount)
    {
        AddToLog(message, "Links count reached maximum, no more links will be added");
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

Q_INVOKABLE void QmlInteractions::stopSearch()
{
    _stopPressed = true;
}

Q_INVOKABLE void QmlInteractions::pauseSearch()
{
    _pausePressed = !_pausePressed;
}

Q_INVOKABLE int QmlInteractions::getRecommendedThreadCount()
{
    return QThread::idealThreadCount();
}

bool QmlInteractions::StopPressed()
{
    return _stopPressed;
}

bool QmlInteractions::PausePressed()
{
    return _pausePressed;
}

QmlInteractions::~QmlInteractions()
{
//    for(int i = _threads.size(); i >= 0; i--)
//    {
//        if(!_threads[i]->isFinished())
//        {
//            _threads[i]->terminate();
//        }
//    }
}
