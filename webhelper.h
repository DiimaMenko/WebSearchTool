#ifndef WEBHELPER_H
#define WEBHELPER_H
#include <QtNetwork>

class WebHelper
{
public:
    WebHelper();
    static QString DownloadHtml(QString address);
    static QList<QString> GetAllLinksFromHtmlBody(QString &htmlbody);
};

#endif // WEBHELPER_H
