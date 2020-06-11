#include "engine.h"
#include <QApplication>
#include <QFile>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Engine w;

    QMainWindow* mw = new QMainWindow();
    QWidget* main = new QWidget();
    main->setLayout(new QVBoxLayout());
    mw->setCentralWidget(main);
    mw->resize(1200, 800);
    mw->show();

    /*
    QWidget* main = new QWidget();
    main->setLayout(new QVBoxLayout());
    main->resize(1200, 800);
    main->show();
    */

    w.registerWidget("mainWindow", main, true);
    w.registerWidget("mainMenu", mw->menuBar(), true);
    w.registerWidget("mainStatusBar", mw->statusBar(), true);
    w.loadHtmlFile("./index.html");
    w.runScriptFile("./dist/index.js");
    w.showInspector(true);

    QFile css("./style.qss");
    if (css.open(QIODevice::ReadOnly)) {
        qobject_cast<QApplication*>(QApplication::instance())->setStyleSheet(css.readAll());
    }

    return app.exec();
}