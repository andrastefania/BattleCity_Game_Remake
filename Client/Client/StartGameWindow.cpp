#include "StartGameWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QPalette>
#include <QImage>
#include <QKeyEvent>

StartGameWindow::StartGameWindow(QWidget* parent)
    : QWidget(parent)
    , textLabel(new QLabel("Una Nueva Galaxia - Beyond the Stars", this))
    , startGameButton(new QPushButton("Start Game", this))
{
    qDebug() << "StartGameWindow constructor called.";
    qDebug() << "Current working directory:" << QDir::currentPath();

   BackgroundHelper:: InitializeBackground(this);

    setAutoFillBackground(true);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet(
        "font-size: 36px; "
        "color: white; "
        "font-weight: bold;"
    );

    QString buttonStyle =
        "font-size: 18px; "
        "color: white; "
        "background-color: rgba(0, 0, 128, 200); "
        "border: 2px solid white; "
        "border-radius: 10px; "
        "padding: 10px;";
    startGameButton->setStyleSheet(buttonStyle);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(textLabel);
    mainLayout->addWidget(startGameButton);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);
    setLayout(mainLayout);
    setWindowTitle("Start Game");
    resize(800, 600);
    connect(startGameButton, &QPushButton::clicked, this, &StartGameWindow::triggerStartGame);
}


void StartGameWindow::triggerStartGame()
{
    qDebug() << "Start Game triggered!";
    emit startGame(); 
}


void StartGameWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) 
    {
        triggerStartGame(); 
    }
    else 
    {
        QWidget::keyPressEvent(event);
    }
}

void StartGameWindow::resizeEvent(QResizeEvent* event)
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

StartGameWindow::~StartGameWindow() = default;
