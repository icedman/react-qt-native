#pragma once

#include <QTimer>
#include <QWebFrame>
#include <QWebInspector>
#include <QWebPage>
#include <QWebView>
#include <QWidget>

class QAction;

class Engine : public QWidget {
    Q_OBJECT
public:
    Engine(QWidget* parent = 0);

    QWebView* view;
    QWebFrame* frame;
    QWebInspector* inspector;

    QVariant runScript(QString script);
    QVariant runScriptFile(QString path);
    bool loadHtml(QString content);
    bool loadHtmlFile(QString path);
    bool registerWidget(QString id, QWidget* widget, bool persistent);

private:
    QStringList mounts;
    QStringList unmounts;
    QStringList updates;
    QTimer updateTimer;
    QMap<QString, QObject*> registry;

    QObject* createObject(QJsonObject props);
    // QObject* createChildObject(QJsonObject props);
    QObject* applyProps(QObject* obj, QJsonObject props);
    QObject* factory(QString json);
    QObject* unset(QString json);
    // QObject* childFactory(QJsonObject props);

public slots:
    void updateUI(QString dom);
    void mountUI(QString dom);
    void unmountUI(QString dom);
    void showInspector(bool withHtml = false);
    void exit();

private Q_SLOTS:
    void setupEnvironment();
    void renderUI();

    void onChange(QString val);
    void onSubmit();
    void onClick(bool checked);
    void onAction(QAction* action);
};