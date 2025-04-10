#ifndef STARTGAMEWINDOW_H
#define STARTGAMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>
#include <QDir>
#include "BackgroundHelper.h"
class StartGameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartGameWindow(QWidget* parent = nullptr);
    ~StartGameWindow();

signals:
    void startGame(); 

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event)override;

private slots:
    void triggerStartGame(); 

private:
    QLabel* textLabel;
    QPushButton* startGameButton;
};

#endif // STARTGAMEWINDOW_H
