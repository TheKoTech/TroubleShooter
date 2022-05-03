#pragma once
#include "wx/wx.h"
#include "cTaskBarIcon.h"
#include "cFrame.h"

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

	// Todo: модель

	// Обновление UI
	bool UpdateIcon();
	void UpdateText(wxCommandEvent &event);

	// Ивенты
	void OnTaskBarIconLeftClick(wxTaskBarIconEvent& event);
	void OnClosed(wxCloseEvent& event);
	wxDECLARE_EVENT_TABLE();
};

