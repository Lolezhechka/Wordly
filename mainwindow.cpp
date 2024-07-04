#include "mainwindow.h"
#include "MenuHandler.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), game(new Game(this)), checkButton(new QPushButton("Совершить попытку", this)),
    newGameButton(new QPushButton("Начать игру", this)), wordLength(5), currentRow(0), gameStarted(false) {

    wordLengthComboBox = new QComboBox(this);
    wordLengthComboBox->addItem("5 букв", 5);
    wordLengthComboBox->addItem("6 букв", 6);
    wordLengthComboBox->addItem("7 букв", 7);
    wordLengthComboBox->setCurrentIndex(0);

    connect(wordLengthComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onWordLengthChanged(int)));
    connect(checkButton, &QPushButton::clicked, this, &MainWindow::checkGuess);
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::startNewGame);

    mainLayout = new QVBoxLayout;
    gameLayout = new QGridLayout;

    centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    mainLayout->addWidget(new QLabel("Выберите длину слова:"), 0, Qt::AlignTop);
    mainLayout->addWidget(wordLengthComboBox, 0, Qt::AlignTop);
    mainLayout->addWidget(newGameButton, 0, Qt::AlignTop);
    mainLayout->addWidget(checkButton, 0, Qt::AlignTop);
    mainLayout->addLayout(gameLayout);

    setupGameLayout();

    menuHandler = new MenuHandler(this);
    menuHandler->createMenu();
}

MainWindow::~MainWindow() {
    delete game;
}

void MainWindow::setupGameLayout() {
    clearLayout(gameLayout);
    inputs.clear();
    guessHistory.clear();

    currentRow = 0;
    int attempts = wordLength; // use word length as the number of attempts

    for (int i = 0; i < attempts; ++i) {
        QVector<QLineEdit*> row;
        for (int j = 0; j < wordLength; ++j) {
            QLineEdit *input = new QLineEdit(this);
            input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            input->setMaxLength(1);
            input->setAlignment(Qt::AlignCenter);
            input->setReadOnly(true);
            input->setStyleSheet("font-size: 24px;");
            gameLayout->addWidget(input, i, j);
            row.append(input);

            connect(input, &QLineEdit::textChanged, this, &MainWindow::handleInputChange);
        }
        guessHistory.append(row);
    }

    if (!guessHistory.isEmpty()) {
        inputs = guessHistory[currentRow];
    }
}

void MainWindow::clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
        }
        delete item;
    }
}

void MainWindow::checkGuess() {
    if (!gameStarted) {
        QMessageBox::warning(this, "Warning", "Пожалуйста, начните игру!");
        return;
    }

    QString guess;
    for (QLineEdit *input : inputs) {
        guess.append(input->text());
    }

    if (guess.size() != wordLength) {
        QMessageBox::warning(this, "Warning", "Пожалуйста, введите полное слово!");
        return;
    }

    if (!game->isValidWord(guess)) {
        QMessageBox::warning(this, "Invalid Word", "Введенное слово некорректно!");
        return;
    }

    QString result = game->checkGuess(guess);
    QString formattedGuess = game->formatGuess(guess);

    if (result.contains("Congratulations")) {
        QMessageBox::information(this, "Game Over", result);
        gameStarted = false;
        game->resetGame();
        setGameButtonsEnabled(true);
        return;
    } else if (game->isGameOver()) {
        QMessageBox::information(this, "Game Over", result);
        gameStarted = false;
        game->resetGame();
        setGameButtonsEnabled(true);
        return;
    }

    updateGuessHistory(guess, formattedGuess);
    currentRow++;
    if (currentRow < guessHistory.size()) {
        inputs = guessHistory[currentRow];
        for (QLineEdit *input : inputs) {
            input->setReadOnly(false);
        }
    }
}

void MainWindow::startNewGame() {
    gameStarted = true;
    game->resetGame();
    setupGameLayout();
    setGameButtonsEnabled(false);

    for (QLineEdit *input : inputs) {
        input->setReadOnly(false);
    }
}

void MainWindow::onWordLengthChanged(int index) {
    wordLength = wordLengthComboBox->itemData(index).toInt();
    game->setWordLength(wordLength);
    setupGameLayout();
    setGameButtonsEnabled(true);
    gameStarted = false;
}

void MainWindow::updateGuessHistory(const QString &guess, const QString &formattedGuess) {
    for (int i = 0; i < guess.size(); ++i) {
        guessHistory[currentRow][i]->setText(guess[i]);
        if (formattedGuess[i] == 'g') {
            guessHistory[currentRow][i]->setStyleSheet("background-color: green; font-size: 24px;");
        } else if (formattedGuess[i] == 'y') {
            guessHistory[currentRow][i]->setStyleSheet("background-color: yellow; font-size: 24px;");
        } else {
            guessHistory[currentRow][i]->setStyleSheet("background-color: red; font-size: 24px;");
        }
        guessHistory[currentRow][i]->setReadOnly(true);
    }
}

void MainWindow::handleInputChange(const QString &text) {
    QLineEdit *senderInput = qobject_cast<QLineEdit*>(sender());
    if (!senderInput) return;

    if (text.length() == 1) {
        int index = inputs.indexOf(senderInput);
        if (index < inputs.size() - 1) {
            inputs[index + 1]->setFocus();
        }
    }
}

void MainWindow::setGameButtonsEnabled(bool enabled) {
    wordLengthComboBox->setEnabled(enabled);
    newGameButton->setEnabled(enabled);
}
