#pragma once
#include <QtWidgets/QMainWindow>
#include <QJsonArray>
#include <cpr/cpr.h>
#include <string>
#include "MapWidget.h"


class ClientServer
{

public:

	static void connectServer();
	static void StartGameWindow();

	static bool LoginClient(const std::string& clientId);

	static bool RegisterClient(const std::string& clientId);

	static bool JoinGame(const std::string& gameCode, const std::string& username);

	static bool ControlsClient(const std::string& controlsClient);
	
	static std::string GenerateCode(uint8_t level, const std::string& username);
	
	QJsonArray GetMap();
	void FetchAndProcessMap(const std::string& gameCode);


	static void GetGeneralSettings();
	static bool SetGeneralSettings(const std::string& settingsJson);
	static bool SaveSettings(const std::string& volume);

	static void GetInGameSettings();
	static bool SetInGameSettings(const std::string& settingsJson);
	static bool SendKeyPress(int key);

	static void UpdateMapCellPlayer(int xNewe, int yNew, int xLast, int yLast); 
	static void UpdateMapCellWall(int xNewe, int yNew, int xLast, int yLast);
	static void RefreshGameMapIncrementally();
	static bool IsLastPlayer();
	
	static std::string m_username;
	static std::string m_gameCode;
	QJsonArray mapData;
private:
	MapWidget* mapWidget = new MapWidget();
};

