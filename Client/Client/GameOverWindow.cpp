#include "GameOverWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include "BackgroundHelper.h"

GameOverWindow::GameOverWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Game Over");
    resize(800, 600);

    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    activateWindow();
    raise();

    BackgroundHelper::InitializeBackground(this);

    titleLabel = new QLabel("Game Over", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: red;");

    scoreLabel = new QLabel("Your Score: 0", this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font-size: 24px; color: white;");

    messageLabel = new QLabel("Better luck next time!", this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 20px; color: white;");

    startNewGameButton = new QPushButton("Start New Game", this);
    exitGameButton = new QPushButton("Exit", this);
    QString style =
        "font-size: 16px; "
        "color: white; "
        "background-color: rgba(0, 0, 128, 200); "
        "border: 2px solid white; "
        "border-radius: 10px; "
        "padding: 5px;";
    startNewGameButton->setStyleSheet(style);
    exitGameButton->setStyleSheet(style);
    startNewGameButton->setFixedSize(200, 40);
    exitGameButton->setFixedSize(200, 40);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scoreLabel);
    mainLayout->addWidget(messageLabel);
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

    connect(startNewGameButton, &QPushButton::clicked, this, &GameOverWindow::onStartNewGameClicked);
    connect(exitGameButton, &QPushButton::clicked, this, &GameOverWindow::onExitClicked);
}


GameOverWindow::~GameOverWindow() = default;

void GameOverWindow::resizeEvent(QResizeEvent* event)
{
    QImage image(":/StartImage/resources/StartGame.jpg");

    if (!image.isNull())
    {
        QPalette palette;
        palette.setBrush(QPalette::Window, QBrush(image.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)));
        setPalette(palette);
    }
    QWidget::resizeEvent(event);
}

void GameOverWindow::onStartNewGameClicked()
{
    QMessageBox::information(this, "Start New Game", "Starting a new game...");
    emit StartNewGame();
    this->close();
}
void GameOverWindow::onExitClicked()
{
    QMessageBox::information(this, "Exit Game", "Closing game...");
    emit ExitGame();
}

void GameOverWindow::setScore(int score)
{
    scoreLabel->setText(QString("Your Score: %1").arg(score));
}

void GameOverWindow::setMessage(const QString& message)
{
    messageLabel->setText(message);
}