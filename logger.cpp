#include "logger.h"
#include <QThread>
#include <QDateTime>
#include <sstream>
#include <QDebug>

Logger::Logger()
{
    text = "";

    _nextMessage = 0;
    _logFile = new QFile("log.txt");
    if(!_logFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        _logToFile = false;
        return;
    }
    logStream = new QTextStream(_logFile);
    _logToFile = true;
}

void Logger::AddMessage(MessageType messageType, const QString &message)
{
    auto fullMessage = Format(messageType, message);
    lock.lockForWrite();

    text.push_front(fullMessage + "\n");
    emit textLogChanged(fullMessage);

    qDebug() << fullMessage;
    if(_logToFile)
    {
        *logStream << fullMessage << "\n";
        _logFile->flush();
    }
    emit textLogChanged(fullMessage);

    lock.unlock();
}

QString Logger::Format(MessageType messageType, const QString &message)
{
    std::stringstream stream;
    stream << messageType << "\t" << QThread::currentThreadId();

    return QString::fromStdString(stream.str()) + "\t"
             + QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + "\t"
             + message;
}

