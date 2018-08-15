#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // this->setFixedSize(QSize(1366, 768));
    view = new WebView(this);
    view->setGeometry(QRect(0, 20, 1366, 748));

    QString exePath = QCoreApplication::applicationDirPath();
    view->load(QUrl("file:///" + exePath + "/html/index.html"));

    this->recentFile = new RecentFile();
    this->recentFile->hide();

    QObject::connect(view, SIGNAL(loadFinished(bool)), this, SLOT(initPage()));
    QObject::connect(recentFile, SIGNAL(fileOpen(QString)), this, SLOT(openRecentFile(QString)));
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    hasEdit = true;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        ctlPress = true;
    }
    if (event->key() == Qt::Key_S && ctlPress) {
        this->on_actionSave_triggered();
        QMessageBox::information(this, "", QStringLiteral("保存成功"));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (hasEdit) {
        int res = QMessageBox::information(this, tr("Note"), QStringLiteral("当前文件还未保存确认离开？"), QMessageBox::Ok, QMessageBox::Cancel );
        if(res == QMessageBox::Ok) {
            this->recentFile->close();
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        this->recentFile->close();
        event->accept();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    view->setGeometry(QRect(0, 20, size.width(), size.height()));
    view->page()->runJavaScript("resize();");
    //view->reload();
}

void MainWindow::openRecentFile(QString path)
{
    if (hasEdit) {
        int res = QMessageBox::information(this, tr("Note"), QStringLiteral("当前文件还未保存确认新打开新文件？"), QMessageBox::Ok, QMessageBox::Cancel );
        if(res != QMessageBox::Ok) {
            return;
        }
    }

    if (!path.isEmpty()) {
        currentPath = path;
        QFile file(currentPath);

        if (!file.exists()) {
            QMessageBox::information(this, "", QStringLiteral("文件不存在！"));
            recentFile->deleteFile(path);
            return;
        }

        file.open(QIODevice::ReadOnly);
        QString data = file.readAll();

        data = data.replace("'", "&apos;");
        data = data.replace("\"", "&quots;");
        data = data.replace("\r\n", "&rn;");
        data = data.replace("\n", "&rn;");
        file.close();

        this->setWindowTitle(file.fileName());

        QString t = "setMarkdown('" + data + "');";
        view->page()->runJavaScript(t);
        hasEdit = false;
    }
}

void MainWindow::on_actionNew_triggered()
{
    if (hasEdit) {
        //文件处于打开状态并且没有执行过保存 新建时需要询问
        int res = QMessageBox::information(this, tr("Note"), QStringLiteral("当前文件还未保存确认新建？"), QMessageBox::Ok, QMessageBox::Cancel );
        if(res != QMessageBox::Ok) {
            return;
        }
    }
    currentPath.clear();
    this->setWindowTitle(QStringLiteral("Markdown编辑器"));
    view->reload();
    hasEdit = false;
}

/**
 * init File By double click
 * @brief MainWindow::loadFile
 * @param path
 */
void MainWindow::loadFile(QString path)
{
    currentPath = path;
    if (!currentPath.isEmpty()) {
        QFile file(currentPath);

        file.open(QIODevice::ReadOnly);
        QString data = file.readAll();

        data = data.replace("'", "&apos;");
        data = data.replace("\"", "&quots;");
        data = data.replace("\r\n", "&rn;");
        data = data.replace("\n", "&rn;");
        file.close();

        initData = data;

        this->setWindowTitle(file.fileName());

        currentPath = currentPath.replace("\\", "/");

        recentFile->insertFile(currentPath);
    }
}
/**
 *
 * @brief MainWindow::initPage
 */
void MainWindow::initPage()
{
    if (initData.size() >= 0) {
        QString t = "setMarkdown('" + initData + "');";
        view->page()->runJavaScript(t);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (hasEdit) {
        int res = QMessageBox::information(this, tr("Note"), QStringLiteral("当前文件还未保存确认新打开新文件？"), QMessageBox::Ok, QMessageBox::Cancel );
        if(res != QMessageBox::Ok) {
            return;
        }
    }
    QString path = QFileDialog::getOpenFileName(this, "", "E:\\", "MarkDown(*.*)");
    if (!path.isEmpty()) {
        currentPath = path;
        QFile file(currentPath);
        file.open(QIODevice::ReadOnly);
        QString data = file.readAll();

        data = data.replace("'", "&apos;");
        data = data.replace("\"", "&quots;");
        data = data.replace("\r\n", "&rn;");
        data = data.replace("\n", "&rn;");
        file.close();

        this->setWindowTitle(file.fileName());

        QString t = "setMarkdown('" + data + "');";
        view->page()->runJavaScript(t);
        hasEdit = false;
        recentFile->insertFile(currentPath);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (!currentPath.isEmpty()) { //当前不是新建状态
        view->page()->runJavaScript("getMarkdown();", [&](const QVariant &v) {
            QByteArray data = v.toByteArray();
            QFile fp(currentPath);

            fp.open(QIODevice::WriteOnly);
            fp.write(data);
            fp.close();

            hasEdit = false;
            recentFile->insertFile(currentPath);
            QFileInfo fi(currentPath);
            recentFile->setLastDir(fi.absolutePath());
        });
    } else {
        this->on_actionSaveAs_triggered(); //调用另存为
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString dir = recentFile->getLastDir();
    dir = dir.length() == 0 ? "E:\\" : dir;

    QString path = QFileDialog::getSaveFileName(this, "", dir, "MarkDown(*.md)");

    if (!path.isEmpty()) {
        currentPath = path;
        this->on_actionSave_triggered();

        this->setWindowTitle(path);
    }
}

void MainWindow::on_action_recentFile_triggered()
{
    this->recentFile->show();
}

void MainWindow::on_action_resetLastDir_triggered()
{
    this->recentFile->setLastDir("");
    QMessageBox::information(this, "", QStringLiteral("操作完成，已恢复至E:\\"));
}
