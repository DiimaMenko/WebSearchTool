#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QReadWriteLock>

class Logger : public QObject
{
    Q_OBJECT
    QList<QString> _log;
    QReadWriteLock lock;

    QString Format(const QString &message);
public:
    Logger();
    void AddMessage(const QString &message);
    QString GetLastMessage() const;
};

#endif // LOGGER_H
