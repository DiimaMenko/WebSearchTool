#ifndef QMLINTERACTIONS_H
#define QMLINTERACTIONS_H

#include <QObject>
#include "webpage.h"
#include "searchresult.h"

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
    QString searchWord;
    double searchProgress;
    bool stopPressed;

public:
    Q_INVOKABLE QString getLastSearchResultTitle() const;
    Q_INVOKABLE QString getLastSearchResultLink() const;
    Q_INVOKABLE QString getExistingLinks() const;
    Q_INVOKABLE double getSearchProgress() const;
    Q_INVOKABLE void stopSearch();
    explicit QmlInteractions (QObject* parent = 0) : QObject(parent) {}
    Q_INVOKABLE void runSearch(QString searchWord, QString startingUrl, int maximumScanUrls);
    void RunLoop();
    void AddLinks(const QList<QString> &linkList);
    void NotifyProgressChanged();

signals:
    void searchResultsChanged();
    void searchFinished();
    void progressChanged();
};

#endif // QMLINTERACTIONS_H
