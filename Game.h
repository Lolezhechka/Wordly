#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QString>
#include <QVector>

class Game : public QObject {
    Q_OBJECT

public:
    explicit Game(QObject *parent = nullptr);
    void resetGame();
    QString checkGuess(const QString &guess);
    QString formatGuess(const QString &guess) const;
    void setWordLength(int length);
    bool isValidWord(const QString &word) const;
    bool isGameOver() const;
    QString getWord() const;

private:
    void loadWords();
    void selectWord();

    QVector<QString> words;
    QString currentWord;
    int wordLength;
    int attemptsLeft; // maximum number of attempts based on word length
};

#endif // GAME_H
