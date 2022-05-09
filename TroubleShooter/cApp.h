#pragma once
#include "wx/wx.h"
#include "cTaskBarIcon.h"
#include "cFrame.h"
#include "ChartController.h"

// cApp это контроллер приложения.

enum AppStatus;

class cApp : public wxApp
{
public:
	cApp();
	~cApp();

	virtual bool OnInit();

private:
	AppStatus appStatus;
	cFrame* mainFrame;
	cTaskBarIcon* taskBarIcon;
	ChartController chartController;

	// Todo: модель

	// Обновление UI
	bool UpdateIcon();
	void initializeChartSeries();
	void updateChartSerie();

	// Ивенты
	void OnTaskBarIconLeftUp(wxTaskBarIconEvent& event);
	void OnClosed(wxCloseEvent& event);
	wxDECLARE_EVENT_TABLE();
};

