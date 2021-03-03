#include "documenthandler.h"

#include <QtGui/QTextDocument>
#include <QtGui/QTextCursor>
#include <QtGui/QFontDatabase>
#include <QtCore/QFileInfo>

DocumentHandler::DocumentHandler()
    : m_target(0)
    , m_doc(0)
//    , m_cursorPosition(-1)
//    , m_selectionStart(0)
//    , m_selectionEnd(0)
{
}

void DocumentHandler::setTarget(QQuickItem *target)
{
    m_doc = 0;
    m_target = target;
    if (!m_target)
        return;

    QVariant doc = m_target->property("textDocument");
    if (doc.canConvert<QQuickTextDocument*>()) {
        QQuickTextDocument *qqdoc = doc.value<QQuickTextDocument*>();
        if (qqdoc)
            m_doc = qqdoc->textDocument();
    }
    emit targetChanged();
}

void DocumentHandler::setFileUrl(const QUrl &arg)
{
    if (m_fileUrl != arg) {
        m_fileUrl = arg;
        QString fileName = QQmlFile::urlToLocalFileOrQrc(arg);
        if (QFile::exists(fileName)) {
            QFile file("log.txt");
            if (file.open(QFile::ReadOnly)) {
                QByteArray data = file.readAll();
                QTextCodec *codec = QTextCodec::codecForHtml(data);
                setText(codec->toUnicode(data));
                if (m_doc)
                    m_doc->setModified(false);
                emit textChanged();
                emit documentTitleChanged();

                //reset();
            }
        }
        emit fileUrlChanged();
    }
}

QUrl DocumentHandler::fileUrl() const
{
    return m_fileUrl;
}

QString DocumentHandler::text() const
{
    return m_text;
}

//void DocumentHandler::setCursorPosition(int position)
//{
//    if (position == m_cursorPosition)
//        return;

//    m_cursorPosition = position;

//    reset();
//}

//QTextCursor DocumentHandler::textCursor() const
//{
//    if (!m_doc)
//        return QTextCursor();

//    QTextCursor cursor = QTextCursor(m_doc);
//    if (m_selectionStart != m_selectionEnd) {
//        cursor.setPosition(m_selectionStart);
//        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
//    } else {
//        cursor.setPosition(m_cursorPosition);
//    }
//    return cursor;
//}

//void DocumentHandler::setSelectionStart(int position)
//{
//    m_selectionStart = position;
//}

//void DocumentHandler::setSelectionEnd(int position)
//{
//    m_selectionEnd = position;
//}
