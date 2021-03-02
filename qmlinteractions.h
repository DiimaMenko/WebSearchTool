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
    int64_t _maximumScanUrlsCount;
    int64_t _threadsCount;
    int64_t _currentResultIterator;
    QString _searchWord;
    double _searchProgress;
    bool _stopPressed;

    Logger _log;

    QReadWriteLock _lockLinks;
    QReadWriteLock _lockResults;
    QReadWriteLock _lockNextLinkIterator;
    QReadWriteLock _lockFinishedThreadsCount;
    QList<SearchWorker*> _threads;
    int64_t _nextLink;
    int64_t _finishedThreadsCount;

public:
    explicit QmlInteractions (QObject* parent = 0) : QObject(parent) {}
    Q_INVOKABLE QString getLastSearchResultTitle();
    Q_INVOKABLE QString getLastSearchResultLink() const;
    Q_INVOKABLE QString getLastLogMessage();
    Q_INVOKABLE double getSearchProgress() const;
    Q_INVOKABLE void getNextResult();
    Q_INVOKABLE void stopSearch();
    Q_INVOKABLE void runSearch(QString searchWord, QString startingUrl, int maximumScanUrls, int threadsCount);
    Q_INVOKABLE Logger *getLogger(){return &_log;}

    void RunLoop();
    void AddResult(QString title, QString url);
    void AddLinks(const QList<QString> &linkList);
    void NotifyProgressChanged();

    QString GetNextLink();
    void AddToLog(const QString &message);

public slots:
    void ThreadFinishedWork(QString threadId);
//    void SomeThreadFinishedWork();

signals:
    void searchResultsChanged();
    void searchFinished();
    void progressChanged();
    void logUpdated();
};

#endif // QMLINTERACTIONS_H
