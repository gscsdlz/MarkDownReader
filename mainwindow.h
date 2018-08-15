#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#include <QFileInfo>

#include <QDebug>

#include "recentfile.h"
#include "webview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadFile(QString);
    void insertIntoDB(QString);
public slots:
    void initPage();
    void openRecentFile(QString);
protected:
    void closeEvent(QCloseEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_action_recentFile_triggered();

    void on_action_resetLastDir_triggered();

private:
    Ui::MainWindow *ui;
    WebView *view;
    QString currentPath;
    QString initData;
    bool hasEdit = false;
    bool ctlPress = false;
    int closeCount = 0;

    RecentFile *recentFile;
};

#endif // MAINWINDOW_H
