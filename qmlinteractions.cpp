#include "qmlinteractions.h"
#include "webhelper.h"
#include "webpage.h"

Q_INVOKABLE void QmlInteractions::runSearch(QString searchWord, QString startingUrl)
{
    WebPage page(searchWord, startingUrl);
    if(page.IsPhraseFound())
    {
        searchResults.append(page.Url());
    }
}
