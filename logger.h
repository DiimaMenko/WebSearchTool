#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QReadWriteLock>
#include <QFile>
#include <QTextStream>

class Logger : public QObject
{
    Q_OBJECT
    QReadWriteLock lock;
    QFile *_logFile;
    QTextStream *logStream;
    bool _logToFile;

    QReadWriteLock _indexLock;
    int64_t _nextMessage;

    QString Format(const QString &message);
public:
//    Q_PROPERTY(QByteArray textLog READ TextLog NOTIFY textLogChanged)
//    QByteArray textLog;
//    QByteArray TextLog(){
//        return textLog;
//    }

    Logger();
    void AddMessage(const QString &message);
//    QString GetLastMessage();

    ~Logger(){
        if(_logFile->isOpen())
        {
            _logFile->flush();
            _logFile->close();
        }
    }

//signals:
//    void textLogChanged(QString);
};

#endif // LOGGER_H
