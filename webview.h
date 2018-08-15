#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QObject>
#include <QWebEngineView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QDebug>

class WebView : public QWebEngineView
{
public:
    explicit WebView(QWidget *parent);
protected:
   void contextMenuEvent(QContextMenuEvent *);
private:
   QMenu *menu;
   QAction *save;
   QAction *newFile;
   QAction *insertDate;
   QAction *insertTime;
};

#endif // WEBVIEW_H
