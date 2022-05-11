#pragma once
#include "wx/wx.h"
#include "cTaskBarIcon.h"
#include "cFrame.h"
#include "ChartController.h"


enum AppStatus;

// cApp это контроллер приложения.
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

	// UI Методы
	bool UpdateIcon();
	void initializeChartSeries();
	void updateChartSerie();
	void createFrame();
	void closeFrame();

	// Ивенты
	void OnTaskBarIconLeftUp(wxTaskBarIconEvent& event);
	void OnClosed(wxCloseEvent& event);
	void OnTaskBarIconMenuShow(wxCommandEvent& event);
	void OnTaskBarIconMenuClose(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};

