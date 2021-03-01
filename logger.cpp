#include "logger.h"
#include <QThread>
#include <QDateTime>

Logger::Logger()
{
}

void Logger::AddMessage(const QString &message)
{
    lock.lockForWrite();
    _log.append(Format(message));
    lock.unlock();
}

QString Logger::Format(const QString &message)
{
    auto thread_handle = QThread::currentThreadId();

    auto a = QString::number(*reinterpret_cast<int*>(thread_handle));
    auto b = QDateTime::currentDateTime().toString("yyyy.dd.mm, hh:mm:ss.zzz");
    return a + "\t" + b + "\t" + message;
}

QString Logger::GetLastMessage() const
{
    return _log.last();
}
