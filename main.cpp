#include "engine.h"
#include <QApplication>
#include <QFile>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QStringList>
#include <QCommandLineParser>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Engine w;

    QString stylePath = "./style.qss";
    QString entryPath = "./index.js";

    QCommandLineParser parser;
    QCommandLineOption inspectOption(QStringList() << "i" << "inspect", "show web inspector");
    QCommandLineOption htmlOption(QStringList() << "m" << "html", "inspect with html view");
    QCommandLineOption styleOption(QStringList() << "s" << "style", "set custom qt stylesheet", "stylePath");
    QCommandLineOption entryOption(QStringList() << "e" << "entry", "set entry script", "entryPath");
    QCommandLineOption devModeOption(QStringList() << "d" << "develop", "run in development mode");
    parser.addHelpOption();
    parser.addOption(inspectOption);
    parser.addOption(htmlOption);
    parser.addOption(styleOption);
    parser.addOption(entryOption);
    parser.addOption(devModeOption);
    parser.process(app);

    if (parser.isSet(styleOption)) {
        stylePath = parser.value(styleOption);
    }
    if (parser.isSet(entryOption)) {
        entryPath = parser.value(entryOption);
    }

    QString htmlPath = entryPath;
    htmlPath.replace(".js", ".html");
    qDebug() << htmlPath;
    qDebug() << entryPath;

    QMainWindow* mw = new QMainWindow();
    QWidget* main = new QWidget();
    main->setLayout(new QVBoxLayout());
    mw->setCentralWidget(main);
    mw->resize(1200, 800);
    mw->show();

    w.registerWidget("mainWindow", main, true);
    w.registerWidget("menuBar", mw->menuBar(), true);
    w.registerWidget("statusBar", mw->statusBar(), true);

    if (parser.isSet(devModeOption)) {
        w.runDevelopment("http://localhost:1234");
    } else {
        w.loadHtmlFile(htmlPath);
        w.runScriptFile("./dist/" + entryPath);
    }

    if (parser.isSet(inspectOption)) {
        w.showInspector(parser.isSet(htmlOption));
    }

    QFile css(stylePath);
    if (css.open(QIODevice::ReadOnly)) {
        qobject_cast<QApplication*>(QApplication::instance())->setStyleSheet(css.readAll());
    }

    return app.exec();
}