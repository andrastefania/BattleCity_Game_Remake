#ifndef VICTORYWINDOW_H
#define VICTORYWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

class VictoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VictoryWindow(QWidget* parent = nullptr);
    ~VictoryWindow();

    void setPlayerScores(int currentScore, int totalScore); 
    void setLeaderboard(const std::vector<QPair<QString, int>>& leaderboard);

signals:
    void StartNewGame();
    void ExitGame();

private slots:
    void onStartNewGameClicked();
    void onExitClicked();

private:
    QLabel* titleLabel;
    QLabel* scoreLabel;
    QLabel* leaderboardTitleLabel;
    QListWidget* leaderboardWidget;
    QPushButton* startNewGameButton;
    QPushButton* exitGameButton;
};

#endif // VICTORYWINDOW_H