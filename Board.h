#ifndef BOARD_H
#define BOARD_H
#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QMessageBox>

const int mines = 99;
const int ROWS = 16;
const int COLS = 30;

enum class State {
    None,
    Flag,
    Question
};

class MineButton : public QPushButton {
    Q_OBJECT

public:
    explicit MineButton(QWidget *parent = nullptr) : QPushButton(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::RightButton) {
            emit rClicked();
        } else {
            QPushButton::mousePressEvent(event);
        }
    }

signals:
    void rClicked();
};

class Minesweeper : public QMainWindow {
    Q_OBJECT

public:
    Minesweeper(QWidget *parent = nullptr);
    ~Minesweeper();

private slots:
    void lClick(int row, int col);
    void rClick(int row, int col);

private:
    void prepareBoard();
    void setMines();
    void expand(int row, int col);
    void end(bool win);
    void restart();

    bool revealed[ROWS][COLS];
    QGridLayout *gridLayout;
    QVector<QVector<MineButton*>> mineButtons;
    QVector<QVector<bool>> sites;
    QVector<QVector<State>> states;
    int remainingSpaces;
    const int totalSpaces = ROWS * COLS;
};
#endif // BOARD_H
