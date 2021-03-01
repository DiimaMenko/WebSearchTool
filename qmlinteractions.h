#ifndef QMLINTERACTIONS_H
#define QMLINTERACTIONS_H

#include <QObject>
#include <QReadWriteLock>
#include "webpage.h"
#include "searchresult.h"
#include "logger.h"

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

class QmlInteractions: public QObject
{
    Q_OBJECT

    QList<SearchResult> searchResults;
    QList<templink> links;
    int64_t maximumScanUrlsCount;
    int64_t threadsCount;
    QString searchWord;
    double searchProgress;
    bool stopPressed;

    Logger _log;

    QReadWriteLock lockLinks;
    QReadWriteLock lockResults;
    QList<QThread*> _threads;

    void AddToLog(const QString &message);

public:
    explicit QmlInteractions (QObject* parent = 0) : QObject(parent) {}
    Q_INVOKABLE QString getLastSearchResultTitle() const;
    Q_INVOKABLE QString getLastSearchResultLink() const;
    Q_INVOKABLE QString getLastLogMessage() const;
    Q_INVOKABLE double getSearchProgress() const;
    Q_INVOKABLE void stopSearch();
    Q_INVOKABLE void runSearch(QString searchWord, QString startingUrl, int maximumScanUrls, int threadsCount);

    void RunLoop();
    void AddResult(QString title, QString url);
    void AddLinks(const QList<QString> &linkList);
    void NotifyProgressChanged();

signals:
    void searchResultsChanged();
    void searchFinished();
    void progressChanged();
    void logUpdated();
};

#endif // QMLINTERACTIONS_H
