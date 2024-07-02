#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QString>
#include <QStringList>

class Game : public QObject {
    Q_OBJECT

public:
    explicit Game(QObject *parent = nullptr);
    QString checkGuess(const QString &guess);
    void resetGame();
    bool isGameOver() const;
    QString getWord() const;
    void setWordLength(int length);
    QString formatGuess(const QString &guess) const;  // Переместите сюда

private:
    QString currentWord;
    QStringList words;
    int maxAttempts = 6;
    int attemptsLeft;
    int wordLength;

    void selectNewWord();
    void loadWordsFromFile();
};

#endif // GAME_H
