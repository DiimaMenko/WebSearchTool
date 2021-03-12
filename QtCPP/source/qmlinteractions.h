#ifndef QMLINTERACTIONS_H
#define QMLINTERACTIONS_H

#include <QObject>
#include <QReadWriteLock>
#include "webpage.h"
#include "searchresult.h"
#include "logger.h"
#include "searchworker.h"

struct templink{
    bool used;
    QString url;
    bool operator == (const templink& other)
    {
        return other.url == url;
    }

    bool operator != (const templink& other)
    {
        return other.url == url;
    }
};

class SearchWorker;

class QmlInteractions: public QObject
{
    Q_OBJECT

    QList<SearchResult> _searchResults;
    QList<templink> _links;
    int _maximumScanUrlsCount;
    int _threadsCount;
    int _currentResultIterator;
    int _linksProcessed;
    QString _searchWord;
    double _searchProgress;
    bool _stopPressed;
    bool _pausePressed;

    Logger _log;

    QReadWriteLock _logLock;
    QReadWriteLock _lockProgress;
    QReadWriteLock _lockLinks;
    QReadWriteLock _lockResults;
    QReadWriteLock _lockNextLinkIterator;
    QReadWriteLock _lockFinishedThreadsCount;
    QReadWriteLock _waitingThreadsCountLock;
    QList<SearchWorker*> _threads;
    int _nextLink;
    int _finishedThreadsCount;
    int threadsWaiting;

public:
    explicit QmlInteractions (QObject* parent = 0);
    ~QmlInteractions();
    Q_INVOKABLE QString getLastSearchResultTitle();
    Q_INVOKABLE QString getLastSearchResultLink() const;
    Q_INVOKABLE double getSearchProgress() const;
    Q_INVOKABLE void getNextResult();
    Q_INVOKABLE void stopSearch();
    Q_INVOKABLE void pauseSearch();
    Q_INVOKABLE void runSearch(QString searchWord, QString startingUrl, int maximumScanUrls, int threadsCount);
    Q_INVOKABLE Logger *getLogger(){return &_log;}
    Q_INVOKABLE int getRecommendedThreadCount();

    void RunLoop();
    void AddResult(QString title, QString url);
    void AddLinks(const QList<QString> &linkList);
    void UpdateProgress();

    QString GetNextLink();
    void AddToLog(MessageType messageType, const QString &message);
    bool StopPressed();
    bool PausePressed();

public slots:
    void ThreadFinishedWork(QString threadId);

signals:
    void searchResultsChanged();
    void searchFinished();
    void progressChanged();
};

#endif // QMLINTERACTIONS_H
