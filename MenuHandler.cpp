#include "MenuHandler.h"

MenuHandler::MenuHandler(QMainWindow *parent) : QObject(parent), mainWindow(parent) {}

void MenuHandler::createMenu() {
    QMenuBar *menuBar = mainWindow->menuBar();

    QMenu *fileMenu = menuBar->addMenu("Файл");
    QAction *changeBackgroundColorAction = fileMenu->addAction("Изменить цвет фона");
    connect(changeBackgroundColorAction, &QAction::triggered, this, &MenuHandler::changeBackgroundColor);

    QMenu *helpMenu = menuBar->addMenu("Справка");
    QAction *aboutAction = helpMenu->addAction("О разработчиках");
    connect(aboutAction, &QAction::triggered, this, &MenuHandler::showAboutDialog);

    QAction *rulesAction = helpMenu->addAction("Правила игры");
    connect(rulesAction, &QAction::triggered, this, &MenuHandler::showRulesDialog);
}

void MenuHandler::changeBackgroundColor() {
    QColor color = QColorDialog::getColor(Qt::white, mainWindow, "Выберите цвет фона");
    if (color.isValid()) {
        mainWindow->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}

void MenuHandler::showAboutDialog() {
    QMessageBox::information(mainWindow, "О разработчиках", "Эта игра была разработана Голубом Дмитрием и Олегом Лашкевичем.");
}

void MenuHandler::showRulesDialog() {
    QMessageBox::information(mainWindow, "Правила игры", "Правила игры: угадать слово за ограниченное количество попыток.");
}
