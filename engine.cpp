#include <QApplication>
#include <QAction>
#include <QBoxLayout>
#include <QDebug>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QSplitter>
#include <QStyle>
#include <QVBoxLayout>
#include <QWebSettings>

#include "engine.h"

#define BEGIN_UI_DEF(T) \
    if (widget == #T) { \
        T* uiObject = new T();

#define BEGIN_UI_PROPS(T) \
    if (widget == #T) {   \
        T* uiObject = qobject_cast<T*>(obj);

#define END_UI()     \
    return uiObject; \
    }

QObject* Engine::createObject(QJsonObject props)
{
    QString widget = props.value("widget").toString();

    QObject* parentObj = NULL;
    if (props.contains("child") && props.contains("parent")) {
        QString parentId = props.value("parent").toString();
        parentObj = registry.value(parentId);
    }

    BEGIN_UI_DEF(QPushButton)
    connect(uiObject, SIGNAL(clicked(bool)), this, SLOT(onClick(bool)));
    END_UI()

    BEGIN_UI_DEF(QLineEdit)
    connect(uiObject, SIGNAL(textEdited(QString)), this, SLOT(onChange(QString)));
    connect(uiObject, SIGNAL(returnPressed()), this, SLOT(onSubmit()));
    END_UI()

    BEGIN_UI_DEF(QHBoxLayout)
    END_UI()

    BEGIN_UI_DEF(QVBoxLayout)
    END_UI()

    BEGIN_UI_DEF(QMenuBar)
    connect(uiObject, SIGNAL(triggered(QAction*)), this, SLOT(onAction(QAction*)));
    END_UI()

    BEGIN_UI_DEF(QMenu)
    connect(uiObject, SIGNAL(triggered(QAction*)), this, SLOT(onAction(QAction*)));
    if (parentObj) {
        QMenuBar* bar = qobject_cast<QMenuBar*>(parentObj);
        if (bar) {
            bar->addMenu(uiObject);
        }
        QMenu* menu = qobject_cast<QMenu*>(parentObj);
        if (menu) {
            menu->addMenu(uiObject);
        }
    }
    END_UI()

    BEGIN_UI_DEF(QAction)
    if (parentObj) {
        QMenuBar* bar = qobject_cast<QMenuBar*>(parentObj);
        if (bar) {
            bar->addAction(uiObject);
        }
        QMenu* menu = qobject_cast<QMenu*>(parentObj);
        if (menu) {
            menu->addAction(uiObject);
        }
    }
    END_UI()

    BEGIN_UI_DEF(QLabel)
    uiObject->setTextFormat(Qt::RichText);
    uiObject->setTextInteractionFlags(Qt::NoTextInteraction);
    END_UI()

    return NULL;
}

QObject* Engine::applyProps(QObject* obj, QJsonObject props)
{
    QString widget = props.value("widget").toString();

    obj->setProperty("className", props.value("className").toString());
    QWidget* w = qobject_cast<QWidget*>(obj);
    if (w) {
        if (props.contains("style")) {
            QString qss = widget;
            qss += " ";
            qss += props.value("style").toString();
            qss = qss.replace("\"", "");
            // qDebug() << qss;
            w->setStyleSheet(qss);
            // w->style()->unpolish(w);
            // w->style()->polish(w);
            // w->update();
        }
    }

    // widget specific
    BEGIN_UI_PROPS(QPushButton)
    uiObject->setText(props.value("text").toString());
    END_UI()

    BEGIN_UI_PROPS(QLineEdit)
    uiObject->setText(props.value("text").toString());
    END_UI()

    BEGIN_UI_PROPS(QLabel)
    uiObject->setText(props.value("text").toString());
    END_UI()

    BEGIN_UI_PROPS(QMenu)
    uiObject->setTitle(props.value("text").toString());
    END_UI()

    BEGIN_UI_PROPS(QAction)
    uiObject->setText(props.value("text").toString());
    if (props.contains("shortcut")) {
        uiObject->setShortcut(QKeySequence(props.value("shortcut").toString()));
    }
    END_UI()

    return obj;
}

QObject* Engine::factory(QString json)
{
    QByteArray bytes;
    bytes.append(json);
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    QJsonObject obj = doc.object();

    if (!obj.contains("id")) {
        return NULL;
    }

    QWidget* w = NULL;
    QBoxLayout* l = NULL;

    // check if already exists
    QString id = obj.value("id").toString();
    if (registry.contains(id)) {
        return applyProps(registry.value(id), obj);
    }

    QString widget = obj.value("widget").toString();
    QString parentId = obj.value("parent").toString();
    QObject* parentObj = registry.value(parentId);

    QWidget* parent = qobject_cast<QWidget*>(parentObj);
    QBoxLayout* parentLayout = qobject_cast<QBoxLayout*>(parentObj);

    //----------
    // create
    //----------
    QObject* qo = createObject(obj);

    // children need not be added to a layout
    // this has already been done at create
    if (obj.contains("child")) {
        qo->setProperty("id", id);
        registry.insert(id, qo);
        return applyProps(qo, obj);
    }

    //----------
    // add to layout
    //----------
    w = qobject_cast<QWidget*>(qo);
    l = qobject_cast<QBoxLayout*>(qo);
    if (!w && !l) {
        return NULL;
    }

    // widget
    if (w) {
        if (parent) {
            w->setParent(parent);
            if (parent->layout()) {
                parent->layout()->addWidget(w);
                // qDebug() << "widget added to layout";
            } else {
                parent->setLayout(new QVBoxLayout());
                // qDebug() << "parent has no layout";
            }
        }

        bool addToLayout = true;
        if (widget == "QMenuBar") {
            addToLayout = false;
        }

        if (parentLayout && addToLayout) {
            parentLayout->addWidget(w);
            // qDebug() << widget << "added to layout";
        }

        // qDebug() << "created new widget";
        // qDebug() << widget;
        // qDebug() << id;
        w->setProperty("id", id);
        registry.insert(id, w);
        return applyProps(w, obj);
    }

    // layout
    if (parent) {
        if (!parent->layout()) {
            parent->setLayout(l);
        } else {
            // qDebug() << "wrapped" << widget;
            QWidget* wrapper = new QWidget();
            wrapper->setLayout(l);
            parent->layout()->addWidget(wrapper);
            l->setProperty("wrapped", true);
        }
    }

    if (parentLayout) {
        parentLayout->addLayout(l);
        // qDebug() << "layout added to layout";
    }

    // qDebug() << "created new layout";
    // qDebug() << widget;
    // qDebug() << id;
    l->setProperty("id", id);
    registry.insert(id, l);
    return applyProps(l, obj);
}

QObject* Engine::unset(QString json)
{
    QByteArray bytes;
    bytes.append(json);
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    QJsonObject obj = doc.object();

    QString id = obj.value("id").toString();
    if (registry.contains(id)) {
        QObject *qo = registry.value(id);

        QWidget *w = qobject_cast<QWidget*>(qo);
        if (w) {
            w->hide();
            delete w;
            // w->deleteLater();
        }

        registry.remove(id);
        return qo;
    }
    return NULL;
}

Engine::Engine(QWidget* parent)
    : QWidget(parent)
    , updateTimer(this)
{
    QSplitter* splitter = new QSplitter(Qt::Vertical, this);

    QVBoxLayout* box = new QVBoxLayout(this);
    setLayout(box);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    view = new QWebView(this);
    frame = view->page()->mainFrame();

    // connect(frame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(setupEnvironment()));

    inspector = new QWebInspector();
    inspector->setPage(view->page());
    inspector->setVisible(true);

    setupEnvironment();

    box->addWidget(splitter);
    box->setMargin(0);
    box->setSpacing(0);
    splitter->addWidget(view);
    splitter->addWidget(inspector);

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(renderUI()));
    updateTimer.start(50);
}

bool Engine::loadHtml(QString content)
{
    content.replace("<script", "<!--script");
    content.replace("/script>", "/script-->");
    view->setHtml(content);
    return true;
}

bool Engine::loadHtmlFile(QString path)
{
    QFile index(path);
    if (index.open(QIODevice::ReadOnly)) {
        QString content = index.readAll();
        return loadHtml(content);
    }
    return false;
}

bool Engine::registerWidget(QString id, QWidget* widget, bool peristent)
{
    if (registry.contains(id)) {
        return false;
    }

    widget->setProperty("id", id);
    widget->setProperty("persistent", peristent);
    registry.insert(id, (QObject*)widget);
    return true;
}

void Engine::setupEnvironment()
{
    frame->addToJavaScriptWindowObject("engine", this);
}

QVariant Engine::runScript(QString script)
{
    return frame->evaluateJavaScript(script);
}

QVariant Engine::runScriptFile(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        return runScript(file.readAll());
    }
    return QVariant();
}

void Engine::mountUI(QString dom)
{
    mounts.push_front(dom);
    // qDebug() << dom;
}

void Engine::unmountUI(QString dom)
{
    unmounts.push_back(dom);
    // qDebug() << "unmount";
}

void Engine::updateUI(QString dom)
{
    updates.push_back(dom);
    // qDebug() << dom;
}

void Engine::renderUI()
{
    if (!mounts.count() && !updates.count() && !unmounts.count()) {
        return;
    }

    QStringList retry;
    // mounts
    for (auto m : mounts) {
        if (!factory(m)) {
            retry.push_back(m);
        }
    }
    mounts.clear();
    mounts << retry;
    retry.clear();

    // updates
    for (auto u : updates) {
        if (!factory(u)) {
            retry.push_back(u);
        }
    }
    updates.clear();
    updates << retry;

    // unmounts
    for (auto n : unmounts) {
        unset(n);
    }
    unmounts.clear();
}

void Engine::onChange(QString val)
{
    QObject* obj = sender();
    QString id = obj->property("id").toString();
    QString script = "$events[\"onChange-" + id + "\"]({ target: { src: \"" + id + "\", value: \"" + val + "\" }})";
    qDebug() << script;
    runScript(script);
}

void Engine::onSubmit()
{
    QObject* obj = sender();
    QString id = obj->property("id").toString();
    QString val = ""; //
    QString script = "$events[\"onSubmit-" + id + "\"]({ target: { src: \"" + id + "\", value: \"" + val + "\" }})";
    qDebug() << script;
    runScript(script);
}

void Engine::onClick(bool checked)
{
    QObject* obj = sender();
    QString id = obj->property("id").toString();
    QString script = "$events[\"onClick-" + id + "\"]({ target: { src: \"" + id + "\", value: " + (checked ? "true" : "false") + " }})";
    qDebug() << script;
    runScript(script);
}

void Engine::onAction(QAction* action)
{
    // QObject* obj = sender();
    QString id = action->property("id").toString();
    bool checked = false; //
    QString script = "$events[\"onClick-" + id + "\"]({ target: { src: \"" + id + "\", value: " + (checked ? "true" : "false") + " }})";
    qDebug() << script;
    runScript(script);
}

void Engine::showInspector(bool withHtml)
{
    if (withHtml) {
        view->show();
    } else {
        view->hide();
    }

    resize(1200, 800);
    show();
}

void Engine::exit()
{
    qobject_cast<QApplication*>(QApplication::instance())->exit();
}