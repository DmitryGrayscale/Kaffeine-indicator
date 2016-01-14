#include "kaffeine.h"
#include <QtDebug>
#include <QApplication>
#include <QtDBus/QtDBus>

Kaffeine::Kaffeine(QWidget *parent)
{
    (void)parent;
    trayIcon = new QSystemTrayIcon();

    iconEnabled = QIcon(":/icons/caffeine-cup-full.svg");
    iconDisabled = QIcon(":/icons/caffeine-cup-empty.svg");

    contextMenu = new QMenu("Kaffeine-indicator");

    contextMenu->addAction("Toggle screensaver", this, SLOT(on_actionToggle()));
    contextMenu->addAction("Exit", this, SLOT(on_actionQuit()));

    trayIcon->setContextMenu(contextMenu);
    trayIcon->setIcon(iconDisabled);
    trayIcon->showMessage("Kaffeine-indicator", "Kaffeine disabled");

    state = INACTIVE;
    cookie = 0;
}

Kaffeine::~Kaffeine()
{
    trayIcon->hide();
    delete contextMenu;
    delete trayIcon;
}


void Kaffeine::show()
{
    trayIcon->show();
    trayIcon->showMessage("Kaffeine-indicator", "Kaffeine is running");
}

void Kaffeine::on_actionQuit()
{
    QApplication::quit();
}

void Kaffeine::on_actionToggle()
{
    state ? state = INACTIVE : state = ACTIVE;
    state ? trayIcon->setIcon(iconEnabled) : trayIcon->setIcon(iconDisabled);
    state ? disableScreenSaver() : enableScreenSaver();
}

void Kaffeine::disableScreenSaver()
{
    QDBusInterface iface("org.kde.screensaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
    if (iface.isValid())
    {
        QDBusReply<uint> reply = iface.call("Inhibit", "Kaffeine-indicator", "User wished");
        if (reply.isValid())
        {
            cookie = reply.value();
        }
        else
        {
            qCritical() << reply.error().message();
        }
    }
    else
    {
        qCritical() << QDBusConnection::sessionBus().lastError().message();
    }
}

void Kaffeine::enableScreenSaver()
{
    QDBusInterface iface("org.kde.screensaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
    if (iface.isValid())
    {
        QDBusReply<void> reply = iface.call("UnInhibit", cookie);
        if (!reply.isValid())
        {
            qCritical() << reply.error().message();
        }
    }
    else
    {
        qCritical() << QDBusConnection::sessionBus().lastError().message();
    }

}
