#include "logger.h"
#include <QThread>
#include <QDateTime>
#include <sstream>
#include <QDebug>

Logger::Logger()
{
    qDebug() << "Log creation";
    text = "";
    _logToFile = true;
    _nextMessage = 0;

    if(_logToFile)
    {
        _logFile = new QFile("log.txt");
        if(!_logFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
        {
            _logToFile = false;
            return;
        }
        logStream = new QTextStream(_logFile);
        _logToFile = true;
    }
}

Q_INVOKABLE QString Logger::getText(){
    return text;
}

void Logger::AddMessage(MessageType messageType, const QString &message)
{
    auto fullMessage = Format(messageType, message);

    text.push_front(fullMessage + "\n");
    emit textLogChanged(fullMessage);

    qDebug() << fullMessage;
    if(_logToFile)
    {
        if (logStream->status() != QTextStream::Ok)
        {
            qDebug() << "Log file writing error";
        }
        *logStream << fullMessage << "\n";
    }
}

QString Logger::Format(MessageType messageType, const QString &message)
{
    std::stringstream stream;
    stream << QThread::currentThreadId();

    return QString::number(messageType) + "\t"
            + QString::fromStdString(stream.str()) + "\t"
            + QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + "\t"
            + message;
}

Logger::~Logger()
{
    qDebug() << "Log deletion";
    if(_logToFile)
    {
        _logFile->flush();
        _logFile->close();
    }
}
