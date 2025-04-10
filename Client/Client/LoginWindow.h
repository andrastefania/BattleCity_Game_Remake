#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QResizeEvent>
#include "BackgroundHelper.h"

class Client;

class LoginWindow :public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);
signals:
    void Login(const QString& username); 
    void RegisterUser(const QString& username); 

protected:
    void resizeEvent(QResizeEvent* event)override;

private slots:
    void OnLoginClicked();
    void OnRegisterClicked();

private:
    
    QLineEdit* clientInput;
    QPushButton* loginButton;
    QPushButton* registerButton;
    QLabel* imageLabel;
    QLabel* promptLabel;
};

