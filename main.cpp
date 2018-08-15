#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    MainWindow w;
    w.show();

    if (argc >= 2) {  //使用了双击打开文件的方式
        QString path = QTextCodec::codecForName("GB2312")->toUnicode(argv[1]);
        w.loadFile(path);
    }

    return a.exec();
}


