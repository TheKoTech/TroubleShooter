#pragma once
#include "wx/wx.h"
#include <wx/display.h>
#include <wx/taskbar.h>
#include <wx/statline.h>

enum IconId;

class cFrame : public wxFrame
{
public:
	cFrame(wxApp* parent); 
	~cFrame();

	void LoadIcon(wxIcon* icon);
	void OnClosed(wxCloseEvent& event);

private:
	wxApp* parent = nullptr;
	wxFrame* frame = nullptr;
	wxBoxSizer* frameSizer = nullptr;
	wxBoxSizer* stabilitySizer = nullptr;
	wxBoxSizer* scanSizer = nullptr;

	wxPanel* stabilityPanel = nullptr;
	wxStaticText* titleText1 = nullptr;
	
	wxButton* btn = nullptr;

	wxPanel* scanPanel = nullptr;

	void InitializeUI();

	//void OnMouseMove(wxMouseEvent& event);
	wxDECLARE_EVENT_TABLE();
};

