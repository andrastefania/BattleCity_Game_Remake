#include "GameMapWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>

GameMapWindow::GameMapWindow(QWidget* parent)
    : QWidget(parent),
    mapWidget(new MapWidget(this)),
    settingsButton(new QPushButton("Settings", this)),
    titleLabel(new QLabel("Game Map", this))
{
    setWindowTitle("Game Map");
    resize(1000, 700);

    BackgroundHelper::InitializeBackground(this);

    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: white;");

    settingsButton->setStyleSheet(
        "font-size: 16px; "
        "color: white; "
        "background-color: rgba(0, 0, 128, 200); "
        "border: 2px solid white; "
        "border-radius: 10px; "
        "padding: 5px;"
    );
    settingsButton->setFixedSize(120, 40);
    settingsButton->setFocusPolicy(Qt::NoFocus);

    mapWidget->setStyleSheet(
        "background-color: rgba(0, 0, 0, 150); "
        "border: 3px solid white; "
        "border-radius: 10px;"
    );
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(titleLabel, 1); 
    topLayout->addWidget(settingsButton, 0, Qt::AlignRight);

    // Layout principal
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout); 
    mainLayout->addWidget(mapWidget, 2, Qt::AlignCenter); 
    mainLayout->setContentsMargins(10, 10, 10, 10);

    setLayout(mainLayout);
    connect(settingsButton, &QPushButton::clicked, this, &GameMapWindow::onSettingsClicked);
}

GameMapWindow::~GameMapWindow() = default;

void GameMapWindow::resizeEvent(QResizeEvent* event)
{
    adjustMapWidgetSize(event);
    QWidget::resizeEvent(event);
}

void GameMapWindow::displayMap(const QJsonArray& mapData)
{
    if (mapData.isEmpty() || !mapWidget) {
        qDebug() << "Eroare: mapData este gol sau mapWidget nu este inițializat!";
        return;
    }

    mapWidget->setMapData(mapData);
    mapWidget->update(); 
}

void GameMapWindow::adjustMapWidgetSize(QResizeEvent* event)
{
    if (mapWidget) {
        int widgetWidth = width() * 0.9; 
        int widgetHeight = height() * 0.9;
        mapWidget->setFixedSize(widgetWidth, widgetHeight);
    }
}

void GameMapWindow::onSettingsClicked()
{
    emit SettingsClicked();
}

void GameMapWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    emit KeyPressed(event->key());
    QWidget::keyPressEvent(event);
}