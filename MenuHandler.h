#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include <QObject>
#include <QMenuBar>
#include <QMainWindow>
#include <QAction>
#include <QColorDialog>
#include <QMessageBox>

class MenuHandler : public QObject {
    Q_OBJECT

public:
    explicit MenuHandler(QMainWindow *parent = nullptr);
    void createMenu();

private slots:
    void changeBackgroundColor();
    void showAboutDialog();
    void showRulesDialog();

private:
    QMainWindow *mainWindow;
};

#endif // MENUHANDLER_H
