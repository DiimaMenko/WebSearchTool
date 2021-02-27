#ifndef QMLINTERACTIONS_H
#define QMLINTERACTIONS_H

#include <QObject>
#include "webpage.h"

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
    QList<QString> searchResults;
    QList<templink> links;
    int64_t maximumScanUrlsCount;
    QString searchWord;
    double searchProgress;

public:
    Q_INVOKABLE QString getLastSearchResults() const;
    Q_INVOKABLE QString getExistingLinks() const;
    Q_INVOKABLE double getSearchProgress() const;
    explicit QmlInteractions (QObject* parent = 0) : QObject(parent) {}
    Q_INVOKABLE void runSearch(QString searchWord, QString startingUrl, int maximumScanUrls);
    void RunLoop();
    void AddLinks(const WebPage &page);
    void NotifyProgressChanged();
signals:
    void searchResultsChanged();
    void searchFinished();
    void progressChanged();
};

#endif // QMLINTERACTIONS_H
