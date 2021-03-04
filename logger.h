#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QReadWriteLock>
#include <QFile>
#include <QTextStream>

enum MessageType {
    message = 0,
    timing = 1,
    link = 2,
};

class Logger : public QObject
{
    Q_OBJECT
    QReadWriteLock lock;
    QFile *_logFile;
    QTextStream *logStream;
    bool _logToFile;

    QReadWriteLock _indexLock;
    int64_t _nextMessage;

    QString Format(MessageType messageType, const QString &message);
public:
    Q_PROPERTY(QString text READ getText NOTIFY textLogChanged)
    QString text;
    Q_INVOKABLE QString getText(){
        return text;
    }

    Logger();
    void AddMessage(MessageType messageType, const QString &message);

    ~Logger(){
        if(_logFile->isOpen())
        {
            _logFile->flush();
            _logFile->close();
        }
    }

signals:
    void textLogChanged(QString);
};

#endif // LOGGER_H
