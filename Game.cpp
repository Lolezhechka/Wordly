#include "Game.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTime>
#include <QDebug>

Game::Game(QObject *parent) : QObject(parent), attemptsLeft(maxAttempts), wordLength(5) {
    loadWordsFromFile();
    qsrand(QTime::currentTime().msec());
    selectNewWord();
}

QString Game::checkGuess(const QString &guess) {
    attemptsLeft--;
    if (guess == currentWord) {
        return formatGuess(guess) + " Congratulations! You've guessed the word!";
    } else if (attemptsLeft > 0) {
        return formatGuess(guess) + " Incorrect guess. Attempts left: " + QString::number(attemptsLeft);
    } else {
        return formatGuess(guess) + " Game Over! No attempts left.";
    }
}

void Game::resetGame() {
    attemptsLeft = maxAttempts;
    selectNewWord();
}

bool Game::isGameOver() const {
    return attemptsLeft <= 0;
}

QString Game::getWord() const {
    return currentWord;
}

void Game::setWordLength(int length) {
    wordLength = length;
    selectNewWord();
}

void Game::selectNewWord() {
    QStringList filteredWords;
    for (const QString& word : words) {
        if (word.length() == wordLength) {
            filteredWords.append(word);
        }
    }

    if (filteredWords.isEmpty()) {
        qWarning() << "No words of length" << wordLength << "found!";
        currentWord = "";  // Set to an empty string or a default word
        return;
    }

    int index = qrand() % filteredWords.size();
    currentWord = filteredWords[index];
}

void Game::loadWordsFromFile() {
    QFile file(":/words.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open words.json file";
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject json = doc.object();
    QJsonArray jsonArray = json["words"].toArray();

    for (int i = 0; i < jsonArray.size(); ++i) {
        words.append(jsonArray[i].toString());
    }
}

QString Game::formatGuess(const QString &guess) const {
    QString result;
    for (int i = 0; i < guess.size(); ++i) {
        if (guess[i] == currentWord[i]) {
            result += "g";  // Green for correct position
        } else if (currentWord.contains(guess[i])) {
            result += "y";  // Yellow for correct letter but wrong position
        } else {
            result += "r";  // Red for incorrect letter
        }
    }
    return result;
}
