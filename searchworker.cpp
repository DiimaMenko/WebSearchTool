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
        dataPointer->AddToLog("Starting work with link: " + link);

        WebPage page(_searchWord, link);
        dataPointer->AddLinks(page.GetLinks());

        if(page.IsPhraseFound())
        {
            dataPointer->AddToLog("+Phrase found on \"" + page.Title() + "\" (" + page.Url() + ")");
            dataPointer->AddResult(page.Title(), page.Url());
        }
        else
        {
            dataPointer->AddToLog("-Phrase is not found on \"" + page.Title() + "\" (" + page.Url() + ")");
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
