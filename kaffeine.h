#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class Kaffeine : public QObject
{
    Q_OBJECT

public:
    explicit Kaffeine(QWidget *parent = 0);
    ~Kaffeine();
    void show();

private:
    QSystemTrayIcon *trayIcon;
    QIcon iconEnabled;
    QIcon iconDisabled;
    QMenu *contextMenu;
    uint cookie;

    enum states {INACTIVE, ACTIVE} state;

    void disableScreenSaver();
    void enableScreenSaver();
private slots:
    void on_actionQuit();
    void on_actionToggle();
};

#endif // MAINWINDOW_H
