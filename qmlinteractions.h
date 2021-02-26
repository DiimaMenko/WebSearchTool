#ifndef QMLINTERACTIONS_H
#define QMLINTERACTIONS_H

#include <QObject>

class QmlInteractions: public QObject{
    Q_OBJECT
    Q_PROPERTY(QList<QString> searchResults READ searchResults WRITE setsearchResults NOTIFY searchResultsChanged)
public:
    QList<QString> searchResults;
    explicit QmlInteractions (QObject* parent = 0) : QObject(parent) {}
    Q_INVOKABLE void runSearch(QString searchWord, QString startingUrl);

signals:
    void searchResultsChanged(){}
};

#endif // QMLINTERACTIONS_H
