#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
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
    void handleInputChange(const QString &text);

private:
    void setupGameLayout();
    void clearLayout(QLayout *layout);
    void updateGuessHistory(const QString &guess, const QString &formattedGuess);
    void setGameButtonsEnabled(bool enabled);

    Game *game;
    QVBoxLayout *mainLayout;
    QGridLayout *gameLayout;
    QWidget *centralWidget;
    QPushButton *checkButton;
    QPushButton *newGameButton;
    QComboBox *wordLengthComboBox;
    QVector<QVector<QLineEdit*>> guessHistory;
    QVector<QLineEdit*> inputs;
    int wordLength;
    int currentRow;
    bool gameStarted;

    MenuHandler *menuHandler;
};

#endif // MAINWINDOW_H
