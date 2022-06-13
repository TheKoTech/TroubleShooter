#pragma once
#include "wx/wx.h"
#include "cTaskBarIcon.h"
#include "cFrame.h"
#include "cSettingsFrame.h"
#include "ChartController.h"
#include "PingController.h"
#include "ConfigController.h"
#include "Ping.h"
#include "app_ids.h"
#include <list>

enum AppStatus { green, yellow, red, black };

// cApp это контроллер приложения.
class cApp : public wxApp
{
public:
	cApp();
	~cApp();

	// Должен быть добавлен приложением и инициализировать "TopLevelWindow" (taskBarIcon в нашем случае). По сути, используем вместо конструктора. 
	virtual bool OnInit();

private:
	// AppStatus отражает статус соединения
	// The AppStatus represents the current connection status: OK (green), Unstable (yellow), Outage (Red) or that the App is disabled (black)
	AppStatus appStatus = green;
	// the main window
	cFrame* mainFrame = nullptr;
	// the settings window
	cSettingsFrame* settingsFrame = nullptr;
	cTaskBarIcon* taskBarIcon = nullptr;
	ChartController chartController;
	PingController* mainPingThread = nullptr;

	// Todo: методы модели

	// UI Методы

	//Стоит вызывать при изменении AppStatus. Меняет иконку приложения на соответствующую AppStatus.
	//Should be called on AppStatus change. Changes the app icon to match AppStatus.
	bool UpdateIcon();

	// Этот метод вызывается при создании cFrame. Он принимает данные из файла с логами, формирует их в wxVector и подаёт чарт контроллеру для отображения.
	// This method is called on cFrame creation. It receives log data from a file, forms it into a wxVector and passes it to chartController for display.
	void initializeChartSeries();
	// This method collects the saved list of Addresses and starts the main pinging thread
	void initializePingController();
	void createFrame();
	void closeFrame();
	void createSettingsFrame();
	void closeSettingsFrame();

	// Ивенты
	void OnTaskBarIconLeftUp(wxTaskBarIconEvent& event);
	void OnClosed(wxCloseEvent& event);
	void OnTaskBarIconMenuShow(wxCommandEvent& event);
	void OnTaskBarIconMenuSettings(wxCommandEvent& event);
	void OnTaskBarIconMenuClose(wxCommandEvent& event);
	void OnPanelLeftUp(wxMouseEvent& event);
	void OnApplyButtonLeftUp(wxCommandEvent& event);

	// todo: Processes the ping results of the main ping thread
	void OnThreadUpdate(wxThreadEvent& event);
	// todo: Cleanup
	void OnThreadCompleted(wxThreadEvent& event);

	wxDECLARE_EVENT_TABLE();
};

