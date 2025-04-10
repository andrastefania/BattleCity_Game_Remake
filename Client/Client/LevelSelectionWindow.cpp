#include "LevelSelectionWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QImage>
#include <QDebug>

LevelSelectionWindow::LevelSelectionWindow(QWidget* parent)
    : QWidget(parent)
    , titleLabel(new QLabel("Choose Difficulty", this))
    , easyButton(new QPushButton("Easy", this))
    , mediumButton(new QPushButton("Medium", this))
    , hardButton(new QPushButton("Hard", this))
{
    setWindowTitle("Select Level");
    resize(600, 400);

    BackgroundHelper::InitializeBackground(this);

    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 24px; "
        "color: white; "
        "font-weight: bold; "
        "margin-bottom: 20px;"
    );

    QString buttonStyle =
        "font-size: 18px; "
        "color: white; "
        "background-color: rgba(0, 0, 128, 200); "
        "border: 2px solid white; "
        "border-radius: 10px; "
        "padding: 10px;";
    easyButton->setStyleSheet(buttonStyle);
    mediumButton->setStyleSheet(buttonStyle);
    hardButton->setStyleSheet(buttonStyle);

    
    easyButton->setFixedSize(100, 50);
    mediumButton->setFixedSize(100, 50);
    hardButton->setFixedSize(100, 50);

    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20); 
    buttonLayout->addStretch();
    buttonLayout->addWidget(easyButton);
    buttonLayout->addWidget(mediumButton);
    buttonLayout->addWidget(hardButton);
    buttonLayout->addStretch();

    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);

    connect(easyButton, &QPushButton::clicked, this, [this]() {
        emit LevelEasy(m_username);
        this->close(); 
        });

    connect(mediumButton, &QPushButton::clicked, this, [this]() {
        emit LevelMediu(m_username);
        this->close();
        });

    connect(hardButton, &QPushButton::clicked, this, [this]() {
        emit LevelHard(m_username);
        this->close();
        });
}

LevelSelectionWindow::~LevelSelectionWindow() = default;

void LevelSelectionWindow::resizeEvent(QResizeEvent* event)
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
