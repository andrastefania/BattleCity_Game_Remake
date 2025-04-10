#include "VictoryWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include "BackgroundHelper.h"

VictoryWindow::VictoryWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Victory");
    resize(800, 600);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    activateWindow(); 
    raise();

    BackgroundHelper::InitializeBackground(this);

    titleLabel = new QLabel("Victory!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: green;");

    scoreLabel = new QLabel("Your Score: Current: 0, Total: 200", this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font-size: 24px; color: white;");

    leaderboardTitleLabel = new QLabel("Leaderboard", this);
    leaderboardTitleLabel->setAlignment(Qt::AlignCenter);
    leaderboardTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");

    leaderboardWidget = new QListWidget(this);
    leaderboardWidget->setStyleSheet(
        "font-size: 18px; color: white; background: transparent; border: none;"
        "QListWidget::item { padding: 5px; background: transparent; }"
        "QListWidget::item:selected { background: transparent; }" 
    );
    leaderboardWidget->setFixedWidth(400);
    leaderboardWidget->setFixedHeight(200);

    startNewGameButton = new QPushButton("Start New Game", this);
    exitGameButton = new QPushButton("Exit", this);
    QString buttonStyle =
        "font-size: 16px; color: white; background-color: rgba(0, 0, 128, 200); "
        "border: 2px solid white; border-radius: 10px; padding: 5px;";
    startNewGameButton->setStyleSheet(buttonStyle);
    exitGameButton->setStyleSheet(buttonStyle);

    startNewGameButton->setFixedSize(200, 40);
    exitGameButton->setFixedSize(200, 40);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scoreLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(leaderboardTitleLabel, 0, Qt::AlignCenter); 
    mainLayout->addWidget(leaderboardWidget, 0, Qt::AlignCenter);    
    mainLayout->addSpacing(20);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(startNewGameButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(exitGameButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    connect(startNewGameButton, &QPushButton::clicked, this, &VictoryWindow::onStartNewGameClicked);
    connect(exitGameButton, &QPushButton::clicked, this, &VictoryWindow::onExitClicked);
}

VictoryWindow::~VictoryWindow() = default;

void VictoryWindow::setPlayerScores(int currentScore, int totalScore)
{
    scoreLabel->setText(QString("Your Score: Current: %1, Total: %2").arg(currentScore).arg(totalScore));
}

void VictoryWindow::setLeaderboard(const std::vector<QPair<QString, int>>& leaderboard)
{
    leaderboardWidget->clear();
    for (const auto& player : leaderboard) {
        leaderboardWidget->addItem(QString("%1: %2").arg(player.first).arg(player.second));
    }
}

void VictoryWindow::onStartNewGameClicked()
{
    QMessageBox::information(this, "Start New Game", "Starting a new game...");
    emit StartNewGame();
    this->close();
}

void VictoryWindow::onExitClicked()
{
    QMessageBox::information(this, "Exit Game", "Closing game...");
    emit ExitGame();
}