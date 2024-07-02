#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QWidget>
#include "Game.h"
#include "MenuHandler.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void checkGuess();
    void startNewGame();
    void onWordLengthChanged(int index);

private:
    Game *game;
    MenuHandler *menuHandler;
    QVector<QVector<QLineEdit*>> guessHistory;
    QVector<QLineEdit*> inputs;
    int wordLength;
    int currentRow;
    bool gameStarted;

    QVBoxLayout *mainLayout;
    QGridLayout *gameLayout;
    QWidget *centralWidget;
    QPushButton *checkButton;
    QPushButton *newGameButton;
    QComboBox *wordLengthComboBox;

    void setupGameLayout();
    void clearLayout(QLayout *layout);
    void updateGuessHistory(const QString &guess, const QString &formattedGuess);
};

#endif // MAINWINDOW_H
