#ifndef WEBHELPER_H
#define WEBHELPER_H
#include <QtNetwork>

class WebHelper
{
public:
    WebHelper();
    static QString DownloadHtml(QString address);
    static QString DownloadHtmlWebEngine(QString address);
};

#endif // WEBHELPER_H
