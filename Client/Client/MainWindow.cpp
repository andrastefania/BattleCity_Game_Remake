#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>


#include "ClientServer.h"
#include "StartGameWindow.h"
#include "LoginWindow.h"
#include "ControlChoiceWindow.h"
#include "CreateOrJoinGameWindow.h"
#include "LevelSelectionWindow.h"
#include "GameMapWindow.h"
#include "InGameSettingsWindow.h"
#include "GeneralSettingsWindow.h"
#include "DisplayCodeWindow.h"
#include "GameOverWindow.h"
#include "VictoryWindow.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connectServer();
    StartGameWindoww();
}

MainWindow::~MainWindow() {
    delete ui;
    delete mapRefreshTimer; 

}

void MainWindow::connectServer() {
    try {
        ClientServer::connectServer();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::StartGameWindoww() {
    try {
        StartGameWindow* startGameWindow = new StartGameWindow(this);
        setCentralWidget(startGameWindow);
        connect(startGameWindow, &StartGameWindow::startGame, this, &MainWindow::onStartGameClicked);
        ClientServer::StartGameWindow();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::onStartGameClicked() {
    LogUserWindow();
}

void MainWindow::LogUserWindow()
{
    try {
        LoginWindow* loginWindow = new LoginWindow();
        loginWindow->show();

        connect(loginWindow, &LoginWindow::Login, this, &MainWindow::HandleLogin);
        connect(loginWindow, &LoginWindow::RegisterUser, this, &MainWindow::HandleRegister);
        close();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::HandleLogin(const QString& username)
{
    std::string stdUsername = username.toUtf8().constData();

    if (ClientServer::LoginClient(stdUsername)) {
        QMessageBox::information(this, "Login", "Welcome, " + username + "!");
        CreateJoinWindow();
        close();
    }
    else {
        QMessageBox::warning(this, "Error", "Login failed. This name does not exist.");
    }

}

void MainWindow::HandleRegister(const QString& username)
{
    std::string stdUsername = username.toUtf8().constData();

    if (ClientServer::RegisterClient(stdUsername)) {
        QMessageBox::information(this, "Joined", "Welcome, " + username + "!");
        ControlChoiceWindow* controlWindow = new ControlChoiceWindow(username);
        controlWindow->show();
        connect(controlWindow, &ControlChoiceWindow::ControlsSet, this, &MainWindow::HandleControlsSet);

    }
    else {
        QMessageBox::warning(this, "Error", "Registration failed. Please try again.");
    }
}


void MainWindow::HandleControlsSet(const QMap<QString, int>& controls)
{
    QJsonObject jsonObject;
    for (auto it = controls.begin(); it != controls.end(); ++it) {
        jsonObject.insert(it.key(), it.value());
    }
    QJsonDocument jsonDoc(jsonObject);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);
    QMessageBox::information(this, "JSON Sent to Server", "The following JSON was sent:\n\n" + jsonString);


    std::string controlsStr = jsonString.toUtf8().constData();
    if (ClientServer::ControlsClient(controlsStr))
    {
        QMessageBox::information(this, "Success", "Controls have been successfully set!");
        CreateJoinWindow();
    }
    else
        QMessageBox::warning(this, "Error", "Failed to set controls: ");


}

void MainWindow::CreateJoinWindow()
{
    CreateOrJoinGameWindow* createJoinWindow = new CreateOrJoinGameWindow();
    createJoinWindow->show();
    connect(createJoinWindow, &CreateOrJoinGameWindow::GeneralSettings, this, &MainWindow::HandleGeneralSettings);
    connect(createJoinWindow, &CreateOrJoinGameWindow::Generate, this, &MainWindow::HandleCreateCode);
    connect(createJoinWindow, &CreateOrJoinGameWindow::CheckCode, this, &MainWindow::HandleCheckCode);
}

void MainWindow::HandleCreateCode(const QString& username)
{
    LevelSelectionWindow* levelWindow = new LevelSelectionWindow();
    levelWindow->show();

    connect(levelWindow, &LevelSelectionWindow::LevelEasy, this, &MainWindow::HandleLevel1);
    connect(levelWindow, &LevelSelectionWindow::LevelMediu, this, &MainWindow::HandleLevel2);
    connect(levelWindow, &LevelSelectionWindow::LevelHard, this, &MainWindow::HandleLevel3);

}

void MainWindow::HandleCheckCode(const QString& gameCode, const QString& username)
{
    if (ClientServer::JoinGame(gameCode.toUtf8().constData(), username.toUtf8().constData())) {
        GameWindow(gameCode.toUtf8().constData());
    }
    else {
        QMessageBox::warning(this, "Error", "Invalid code. Please try again.");
    }
}

void MainWindow::HandleLevel1(const QString& username)
{
    std::string gameCode = ClientServer::GenerateCode(1, username.toUtf8().constData());

    if (gameCode.empty()) {
        QMessageBox::information(this, "Error", "Failed to generate code.");
    }
    else {
        GameWindow(gameCode.c_str());
        DisplayCode(QString(gameCode.c_str()));
    }

}

void MainWindow::HandleLevel2(const QString& username)
{
    std::string gameCode = ClientServer::GenerateCode(2, username.toUtf8().constData());

    if (gameCode.empty()) {
        QMessageBox::information(this, "Error", "Failed to generate code.");
    }
    else {
        GameWindow(gameCode.c_str());
        DisplayCode(QString(gameCode.c_str()));
    }
}
void MainWindow::HandleLevel3(const QString& username)
{
    std::string gameCode = ClientServer::GenerateCode(3, username.toUtf8().constData());

    if (gameCode.empty()) {
        QMessageBox::information(this, "Error", "Failed to generate code.");
    }
    else {
        GameWindow(gameCode.c_str());
        DisplayCode(QString(gameCode.c_str()));
    }
}

void MainWindow::GameWindow(const QString& gameCode)
{
    
        GameMapWindow* gameMapWindow = new GameMapWindow();
        gameMapWindow->show();

        mapRefreshTimer = new QTimer(this);
        connect(mapRefreshTimer, &QTimer::timeout, this, &ClientServer::RefreshGameMapIncrementally);
        mapRefreshTimer->start(500);

        connect(gameMapWindow, &GameMapWindow::KeyPressed, this, &MainWindow::HandleKeyPressedOnMap);
        connect(gameMapWindow, &GameMapWindow::SettingsClicked, this, &MainWindow::HandleInGameSettings);

        ClientServer client;
        client.FetchAndProcessMap(gameCode.toUtf8().constData());

        try {
            QJsonArray mapData = client.GetMap();

            qDebug() << "Conținutul mapData obținut de la server:";
            for (const auto& row : mapData) {
                qDebug() << row;
            }

            gameMapWindow->displayMap(mapData);
        }
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
}

void MainWindow::HandleKeyPressedOnMap(int key)
{
    if (ClientServer::SendKeyPress(key))
    {
       qDebug() << "Key press successfully sent to server.";
    }
    else {
       qDebug() << "Failed to send key press to server.";
    }
}


void MainWindow::DisplayCode(const QString& message)
{
    DisplayCodeWindow* displayCode = new DisplayCodeWindow();
    displayCode->setMessage(message);
    displayCode->show();

}

void MainWindow::HandleInGameSettings()
{
    InGameSettingsWindow* settingsWindow = new InGameSettingsWindow(this);
    settingsWindow->positionInTopRight(this);
    settingsWindow->show();

    connect(settingsWindow, &InGameSettingsWindow::backToGame, this, &MainWindow::HandleBackToGameSetting);
    connect(settingsWindow, &InGameSettingsWindow::exitGame, this, &MainWindow::HandleExitGameSetting);
    connect(settingsWindow, &InGameSettingsWindow::SaveSettings, this, &MainWindow::HandleSaveSettings);
}

void MainWindow::HandleBackToGameSetting() {
    qDebug() << "Back to game!";
}

void MainWindow::HandleExitGameSetting() {
    if (ClientServer::IsLastPlayer())
    {
        qDebug() << "Exit game!";
        VictoryWindow* victoryGame = new VictoryWindow(this);
        victoryGame->show();
        connect(victoryGame, &VictoryWindow::StartNewGame, this, &MainWindow::CreateJoinWindow);
        connect(victoryGame, &VictoryWindow::ExitGame, this, &MainWindow::HandleCloseGame);
    }
    else
    {
        qDebug() << "Exit game!";
        GameOverWindow* gameOver = new GameOverWindow(this);
        gameOver->show();
        connect(gameOver, &GameOverWindow::StartNewGame, this, &MainWindow::CreateJoinWindow);
        connect(gameOver, &GameOverWindow::ExitGame, this, &MainWindow::HandleCloseGame);
    }
}

void MainWindow::HandleCloseGame()
{
    QApplication::quit();
}

void MainWindow::HandleSaveSettings(int volume)
{

    qDebug() << "Save settings!";
    std::string volumeStr = std::to_string(volume);

    if (ClientServer::SaveSettings(volumeStr)) {
        QMessageBox::information(this, "Success", "General settings have been saved to the server!");
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to save general settings to the server.");
    }

}

void MainWindow::HandleEditControls()
{
    qDebug() << "Edit Controls!";

}

void MainWindow::HandleLogOut()
{
    qDebug() << "LogOut!";

}

void MainWindow::HandleDeleteAccount()
{
    qDebug() << "Delete Account!";

}

void MainWindow::HandleGeneralSettings()
{
    GeneralSettingsWindow* generalSettings = new GeneralSettingsWindow(this);
    generalSettings->show();

    connect(generalSettings, &GeneralSettingsWindow::backToGame, this, &MainWindow::HandleBackToGameSetting);
    connect(generalSettings, &GeneralSettingsWindow::SaveSettings, this, &MainWindow::HandleSaveSettings);
    connect(generalSettings, &GeneralSettingsWindow::EditControls, this, &MainWindow::HandleEditControls);
    connect(generalSettings, &GeneralSettingsWindow::Logout, this, &MainWindow::HandleLogOut);
    connect(generalSettings, &GeneralSettingsWindow::Delete, this, &MainWindow::HandleDeleteAccount);
}
