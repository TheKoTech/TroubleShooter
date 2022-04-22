#pragma once
#include "wx/wx.h"
#include "cTaskBarIcon.h"
#include "cFrame.h"

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
	void OnTaskBarIconLeftClick(wxTaskBarIconEvent& event);
	void OnClosed(wxCloseEvent& event);
	wxDECLARE_EVENT_TABLE();

	bool UpdateIcon();

};

