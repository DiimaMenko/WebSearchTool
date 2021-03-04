#include "webhelper.h"

WebHelper::WebHelper()
{

}

QString WebHelper::DownloadHtml(QString address)
{
    QNetworkAccessManager manager;
    manager.setTransferTimeout(10000);

    QUrl url(address);
    QNetworkRequest request(url);
    request.setTransferTimeout(10000);

    QNetworkReply *reply(manager.get(request));
    QEventLoop loop;

    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    loop.exec();

    if(reply->error() > 0) {
        reply->deleteLater();
        return "";
    }
    else {
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (v >= 200 && v < 300)
        {
            reply->deleteLater();
            return reply->readAll();
        }
        else
        {
            return "";
        }
    }
}

QString WebHelper::DownloadHtmlWebEngine(QString address)
{

}
