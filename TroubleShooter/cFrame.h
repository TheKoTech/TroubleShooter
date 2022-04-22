#pragma once
#include "wx/wx.h"
#include <wx/taskbar.h>

enum IconId;

class cFrame : public wxFrame
{
public:
	cFrame(wxApp* parent); 
	~cFrame();

	void LoadIcon(wxIcon* icon);
	void OnClosed(wxCloseEvent& event);

private:
	wxApp* parent;
	wxSize* frameSize;

	wxDECLARE_EVENT_TABLE();
};

