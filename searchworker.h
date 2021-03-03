#ifndef SEARCHWORKER_H
#define SEARCHWORKER_H

#include <QThread>
#include "webpage.h"
#include "qmlinteractions.h"

class QmlInteractions;

class SearchWorker : public QThread
{
    Q_OBJECT
private:
    QList<QString> *linksList;
    QmlInteractions *dataPointer;
    QString _searchWord;

protected:
    void run();
public:
    explicit SearchWorker(QmlInteractions *i_dataPointer, QString searchWord);
    void RunAlternative();
    void FinalizeWork();

signals:
    void ThreadFinishedWork(QString str);
};

#endif // SEARCHWORKER_H
