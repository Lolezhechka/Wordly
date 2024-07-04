#include "Game.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTime>
#include <QDebug>
#include <random>
#include <QJsonObject>

Game::Game(QObject *parent)
    : QObject(parent), wordLength(5), attemptsLeft(5) {
    loadWords();
    selectWord();
}

void Game::loadWords() {
    QFile file(":/words.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open words.json file.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray wordsArray = doc.object().value("words").toArray();

    for (const QJsonValue &value : wordsArray) {
        words.append(value.toString().toLower());
    }

    qDebug() << "Words loaded:" << words; // Debug output to check loaded words
}

void Game::selectWord() {
    QVector<QString> filteredWords;
    for (const QString &word : words) {
        if (word.length() == wordLength) {
            filteredWords.append(word);
        }
    }

    qDebug() << "Filtered words of length" << wordLength << ":" << filteredWords; // Debug output

    if (filteredWords.empty()) {
        qWarning("No words of selected length.");
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, filteredWords.size() - 1);

    int index = dis(gen);
    currentWord = filteredWords[index];
    attemptsLeft = wordLength; // Set attempts based on word length

    qDebug() << "Selected word:" << currentWord; // Debug output
}

void Game::resetGame() {
    selectWord();
}

QString Game::checkGuess(const QString &guess) {
    if (guess == currentWord) {
        return "Congratulations! You've guessed the word.";
    }
    attemptsLeft--;
    return attemptsLeft > 0 ? "" : "Game Over. The word was: " + currentWord;
}

QString Game::formatGuess(const QString &guess) const {
    QString formatted;
    for (int i = 0; i < guess.length(); ++i) {
        if (guess[i] == currentWord[i]) {
            formatted.append('g');
        } else if (currentWord.contains(guess[i])) {
            formatted.append('y');
        } else {
            formatted.append('r');
        }
    }
    return formatted;
}

void Game::setWordLength(int length) {
    wordLength = length;
    attemptsLeft = length; // update attempts based on word length
    qDebug() << "Setting word length to" << length; // Debug output
    selectWord();
}

bool Game::isValidWord(const QString &word) const {
    bool valid = words.contains(word.toLower());
    qDebug() << "Checking if word is valid:" << word << "Result:" << valid; // Debug output
    return valid;
}

bool Game::isGameOver() const {
    return attemptsLeft <= 0;
}

QString Game::getWord() const {
    return currentWord;
}
