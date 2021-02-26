#include "webhelper.h"

WebHelper::WebHelper()
{

}

QString WebHelper::DownloadHtml(QString address)
{
    QNetworkAccessManager manager;
    QUrl url(address);
    QNetworkRequest request(url);
    QNetworkReply *reply(manager.get(request));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->readAll();
}
