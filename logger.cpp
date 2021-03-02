#include "logger.h"
#include <QThread>
#include <QDateTime>
#include <sstream>
#include <QDebug>

Logger::Logger()
{
    textLog = "";
    _nextMessage = 0;
    _log.clear();
    _logFile = new QFile("log.txt");
    if(!_logFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        _logToFile = false;
        return;
    }
    logStream = new QTextStream(_logFile);
    _logToFile = true;
}

void Logger::AddMessage(const QString &message)
{
    auto fullMessage = Format(message);
    lock.lockForWrite();
    //_log.append(fullMessage);
    textLog.append(fullMessage.toUtf8() + "\n");
    qDebug() << fullMessage;
    if(_logToFile)
    {
        *logStream << fullMessage << "\n";
        _logFile->flush();
    }
    lock.unlock();
}

QString Logger::Format(const QString &message)
{
    std::stringstream stream;
    stream << QThread::currentThreadId();

    return QString::fromStdString(stream.str()) + "\t"
             + QDateTime::currentDateTime().toString("yyyy.dd.mm hh:mm:ss.zzz") + "\t"
             + message;
}

QString Logger::GetLastMessage()
{
    QString message = "";
    //_indexLock.lockForRead();

    if(_nextMessage < _log.size())
    {
        message= _log[_nextMessage];
        _nextMessage++;
    }

    //_indexLock.unlock();
    return message;
}
