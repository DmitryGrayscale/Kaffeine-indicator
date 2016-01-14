#include "kaffeine.h"
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
void KaffeineMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray timeMsg = QDateTime::currentDateTime().toString().toLocal8Bit();

    switch (type) {
    case QtDebugMsg:
        txt.sprintf("[%s] Debug: %s (%s:%u, %s)\n", timeMsg.constData(), localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        txt.sprintf("[%s] Warning: %s (%s:%u, %s)\n", timeMsg.constData(), localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        txt.sprintf("[%s] Critical: %s (%s:%u, %s)\n", timeMsg.constData(), localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        txt.sprintf("[%s] Fatal: %s (%s:%u, %s)\n", timeMsg.constData(), localMsg.constData(), context.file, context.line, context.function);
        break;
    }

    QDir dir(QDir::homePath() + "/.local/share/");
    dir.mkpath("Kaffeine");

    QFile outFile(QDir::homePath() + "/.local/share/Kaffeine/log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(KaffeineMsgHandler);

    Kaffeine k;
    k.show();

    return a.exec();
}
