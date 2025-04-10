#include "LoginWindow.h"
#include "ClientServer.h"
#include "ControlChoiceWindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QImage>

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
    , promptLabel(new QLabel("Enter your name:", this))
    , clientInput(new QLineEdit(this))
    , loginButton(new QPushButton("Sign In", this))
    , registerButton(new QPushButton("Sign Up", this))
{
    qDebug() << "LoginWindow constructor called.";

   BackgroundHelper::InitializeBackground(this);
    promptLabel->setAlignment(Qt::AlignCenter);
    promptLabel->setStyleSheet(
        "font-size: 30px; "
        "color: white; "
        "font-weight: bold;"
    );

    clientInput->setPlaceholderText("Your name");
    clientInput->setStyleSheet(
        "font-size: 16px; "
        "color: black; "
        "padding: 5px; "
        "border: 2px solid white; "
        "border-radius: 5px; "
        "background-color: rgba(255, 255, 255, 200);"
    );
    clientInput->setFixedSize(300, 40);

    QString buttonStyle =
        "font-size: 18px; "
        "color: white; "
        "background-color: rgba(0, 0, 128, 200); "
        "border: 2px solid white; "
        "border-radius: 10px; "
        "padding: 10px;";
    loginButton->setStyleSheet(buttonStyle);
    registerButton->setStyleSheet(buttonStyle);

    loginButton->setFixedSize(150, 50);
    registerButton->setFixedSize(150, 50);

    QVBoxLayout* inputLayout = new QVBoxLayout();
    inputLayout->addWidget(promptLabel, 0, Qt::AlignCenter);
    inputLayout->addWidget(clientInput, 0, Qt::AlignCenter);
    inputLayout->setSpacing(20);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addSpacing(20); 
    buttonLayout->addWidget(registerButton);
    buttonLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(30); 

    setLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::OnLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::OnRegisterClicked);

    setWindowTitle("Logare");
    resize(800, 600);

    
}


void LoginWindow::resizeEvent(QResizeEvent* event)
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

void LoginWindow::OnLoginClicked()
{
    QString clientId = clientInput->text().trimmed();
    if (clientId.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter a name!");
        return;
    }
    emit Login(clientId); 
    close();
}

void LoginWindow::OnRegisterClicked()
{
    QString clientId = clientInput->text().trimmed();

    if (clientId.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter a name!");
        return;
    }
    emit RegisterUser(clientId);
    deleteLater();
    close();
}
