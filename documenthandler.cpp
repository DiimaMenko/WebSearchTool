//#include "documenthandler.h"

//#include <QtGui/QTextDocument>
//#include <QtGui/QTextCursor>
//#include <QtGui/QFontDatabase>
//#include <QtCore/QFileInfo>

//DocumentHandler::DocumentHandler()
//    : m_target(0)
//    , m_doc(0)
//{
//}

//void DocumentHandler::setTarget(QQuickItem *target)
//{
//    m_doc = 0;
//    m_target = target;
//    if (!m_target)
//        return;

//    QVariant doc = m_target->property("textDocument");
//    if (doc.canConvert<QQuickTextDocument*>()) {
//        QQuickTextDocument *qqdoc = doc.value<QQuickTextDocument*>();
//        if (qqdoc)
//            m_doc = qqdoc->textDocument();
//    }
//    emit targetChanged();
//}

//void DocumentHandler::Update(const QUrl &arg)
//{
//    if (m_fileUrl != arg) {
//        m_fileUrl = arg;
//        QString fileName = QQmlFile::urlToLocalFileOrQrc(arg);
//        if (QFile::exists(fileName)) {
//            QFile file("log.txt");
//            if (file.open(QFile::ReadOnly)) {
//                QByteArray data = file.readAll();
//                m_text = data;
//                if (m_doc)
//                    m_doc->setModified(false);
//                emit textChanged();
//            }
//        }
//    }
//}

//QUrl DocumentHandler::fileUrl() const
//{
//    return m_fileUrl;
//}

//QString DocumentHandler::text() const
//{
//    return m_text;
//}
