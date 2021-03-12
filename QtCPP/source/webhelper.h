#ifndef WEBHELPER_H
#define WEBHELPER_H
#include <QtNetwork>

class WebHelper
{
public:
    WebHelper();
    ~WebHelper();
    static QString DownloadHtml(QString address);
};

#endif // WEBHELPER_H
