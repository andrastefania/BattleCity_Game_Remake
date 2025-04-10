#ifndef GAMEOVERWINDOW_H
#define GAMEOVERWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class GameOverWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameOverWindow(QWidget* parent = nullptr);
    ~GameOverWindow();

    void setScore(int score);
    void setMessage(const QString& message);
signals:
    void StartNewGame();
    void ExitGame();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onStartNewGameClicked();
    void onExitClicked();

private:
    QLabel* titleLabel;
    QLabel* scoreLabel;
    QLabel* messageLabel;
    QPushButton* startNewGameButton;
    QPushButton* exitGameButton;
};

#endif // GAMEOVERWINDOW_H