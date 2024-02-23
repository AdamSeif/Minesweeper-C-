#include "Board.h"
#include <queue>
#include <QTime>
#include <QVector>
#include <QApplication>

Minesweeper::Minesweeper(QWidget *parent)
    : QMainWindow(parent), totalSpaces(ROWS * COLS), remainingSpaces(totalSpaces)
{
    // spaces are unrevealed at the beggining
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            revealed[i][j] = false;
        }
    }

    prepareBoard();
}

Minesweeper::~Minesweeper()
{

}

void Minesweeper::prepareBoard()
{

    // creating layout
    gridLayout = new QGridLayout();
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: white;");

    // aligning vectors
    mineButtons.resize(ROWS, QVector<MineButton*>(COLS));
    sites.resize(ROWS, QVector<bool>(COLS, false));

    // randomizing seed for mine setting
    srand(QTime::currentTime().msec());
    states = QVector<QVector<State>>(ROWS, QVector<State>(COLS, State::None));

    // creating buttons
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            mineButtons[row][col] = new MineButton(this);
            mineButtons[row][col]->setFixedSize(30, 30);
            gridLayout->addWidget(mineButtons[row][col], row, col);
            connect(mineButtons[row][col], &MineButton::clicked, mineButtons[row][col], [this, row, col]() {
                lClick(row, col);
            });
            connect(mineButtons[row][col], &MineButton::rClicked, mineButtons[row][col], [this, row, col]() {
                rClick(row, col);
            });
        }
    }
    // setting mines in their sites
    setMines();
}

void Minesweeper::setMines()
{
    int unplacedMines = mines;

    while (unplacedMines > 0) {
        // random mine setting
        int row = rand() % ROWS;
        int col = rand() % COLS;

        if (!sites[row][col]) {
            sites[row][col] = true;
            --unplacedMines;
        }
    }
}

void Minesweeper::lClick(int row, int col) {
    // handling left clicks
    if (sites[row][col]) {
        //blowing up mines and ending game
        mineButtons[row][col]->setIcon(QIcon(":/Icons/bomb_explode_sm.png"));
        mineButtons[row][col]->setIconSize(QSize(30, 30));
        end(false);
    } else {
        // revealing whitespace
        expand(row, col);

        if (remainingSpaces == mines) {
            end(true);
        }
    }
}

void Minesweeper::rClick(int row, int col) {
    // handling flags and question marks
    if (static_cast<int>(states[row][col]) == static_cast<int>(State::None)) {
        states[row][col] = State::Flag;
        mineButtons[row][col]->setIcon(QIcon(":/Icons/mine_flag.png"));
    } else if (static_cast<int>(states[row][col]) == static_cast<int>(State::Flag)) {
        states[row][col] = State::Question;
        mineButtons[row][col]->setIcon(QIcon(":/Icons/questionmark.png"));
    } else if (static_cast<int>(states[row][col]) == static_cast<int>(State::Question)) {
        states[row][col] = State::None;
        mineButtons[row][col]->setIcon(QIcon());
    }
    mineButtons[row][col]->setIconSize(QSize(30, 30));
}

void Minesweeper::expand(int row, int col)
{
    std::queue<std::pair<int, int>> spacesToReveal;
    spacesToReveal.push({row, col});

    while (!spacesToReveal.empty()) {
        auto [currentRow, currentCol] = spacesToReveal.front();
        spacesToReveal.pop();

        if (currentRow < 0 || currentRow >= ROWS || currentCol < 0 || currentCol >= COLS || revealed[currentRow][currentCol]) {
            continue;
        }

        revealed[currentRow][currentCol] = true;
        mineButtons[currentRow][currentCol]->setFlat(true);
        mineButtons[currentRow][currentCol]->setEnabled(false);
        --remainingSpaces;

        int adjacentMines = 0;

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (currentRow + i >= 0 && currentRow + i < ROWS && currentCol + j >= 0 && currentCol + j < COLS) {
                    if (sites[currentRow + i][currentCol + j]) {
                        ++adjacentMines;
                    }
                }
            }
        }

        if (adjacentMines > 0) {
            mineButtons[currentRow][currentCol]->setText(QString::number(adjacentMines));
        } else {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    spacesToReveal.push({currentRow + i, currentCol + j});
                }
            }
        }
    }
}

void Minesweeper::end(bool win) {
    if(win == false)
    {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (sites[i][j]) {
                    mineButtons[i][j]->setIcon(QIcon(":/Icons/bomb_explode_sm.png"));
                    mineButtons[i][j]->setIconSize(QSize(30, 30));
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (sites[i][j]) {
                    mineButtons[i][j]->setIcon(QIcon(":/Icons/bomb.png"));
                    mineButtons[i][j]->setIconSize(QSize(30, 30));
                }
            }
        }
    }

    QString message = win ? "Eazy Gaaaaame!!" : "Hold this L bro!";
    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Over");
    msgBox.setText(message);

    QPushButton *restartButton = msgBox.addButton("Restart", QMessageBox::YesRole);
    QPushButton *quitButton = msgBox.addButton("Exit", QMessageBox::NoRole);

    msgBox.exec();

    if (msgBox.clickedButton() == restartButton) {
        restart();
    } else if (msgBox.clickedButton() == quitButton) {
        QApplication::quit();
    }
}

void Minesweeper::restart()
{
    // resets board
    remainingSpaces = totalSpaces;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            mineButtons[row][col]->setEnabled(true);
            mineButtons[row][col]->setText("");
            mineButtons[row][col]->setFlat(false);
            mineButtons[row][col]->setIcon(QIcon());
            sites[row][col] = false;
            revealed[row][col] = false;
        }
    }

    setMines();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Minesweeper game;
    game.show();

    return app.exec();
}
