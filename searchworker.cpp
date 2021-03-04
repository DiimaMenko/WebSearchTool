#include "searchworker.h"
#include <sstream>

SearchWorker::SearchWorker(QmlInteractions *i_dataPointer, QString searchWord) : QThread(),
    dataPointer(i_dataPointer),
    _searchWord(searchWord)
{

}

void SearchWorker::run()
{
    do
    {
        QString link = dataPointer->GetNextLink();
        if(link.isEmpty())
        {
           FinalizeWork();
           return;
        }
        while(dataPointer->PausePressed())
        {
            msleep(100);
        }
        dataPointer->AddToLog(MessageType::link, "Starting work with link: " + link);

        WebPage page(_searchWord, link);
        if(page.GetErrorCode() != 0)
        {
            dataPointer->AddToLog(MessageType::message, "An error with code " + QString::number(page.GetErrorCode()) + " occured working with current link. Skipping it.");
            continue;
        }

        dataPointer->AddToLog(timing, "Page size (bytes):     \t" + QString::number(page.pageSize));
        dataPointer->AddToLog(timing, "Overal time:           \t" + QString::number(page.overalTime));
        dataPointer->AddToLog(timing, "Downloading:           \t" + QString::number(page.downloadingTime));
        dataPointer->AddToLog(timing, "Searching title:       \t" + QString::number(page.searchingTitleTime));
        dataPointer->AddToLog(timing, "Getting Body:          \t" + QString::number(page.gettingBodyTime));
        dataPointer->AddToLog(timing, "Getting links:         \t" + QString::number(page.gettingLinksTime));
        dataPointer->AddToLog(timing, "Getting text + search: \t" + QString::number(page.gettingTextFromBodyTime));

        dataPointer->AddLinks(page.GetLinks());

        if(page.IsPhraseFound())
        {
            dataPointer->AddToLog(message, "+Phrase found on \"" + page.Title() + "\" (" + page.Url() + ")");
            dataPointer->AddResult(page.Title(), page.Url());
        }
        else
        {
            dataPointer->AddToLog(message, "-Phrase is not found on \"" + page.Title() + "\" (" + page.Url() + ")");
        }
    }
    while(!dataPointer->StopPressed());
    FinalizeWork();
}

void SearchWorker::FinalizeWork()
{
    std::stringstream stream;
    stream << QThread::currentThreadId();

    emit ThreadFinishedWork(QString::fromStdString(stream.str()));

    exit(0);
}
