#include "webview.h"

WebView::WebView(QWidget *parent)
    :QWebEngineView(parent)
{
    /*menu = new QMenu(this);
    insertDate = new QAction(QStringLiteral("插入当前日期"), this);
    insertTime = new QAction(QStringLiteral("插入当前时间"), this);*/
}

void WebView::contextMenuEvent(QContextMenuEvent *ev)
{
   /* menu->clear();
    menu->addAction(insertDate);
    menu->addAction(insertTime);
    menu->exec(QCursor::pos());

    ev->accept();*/
}

