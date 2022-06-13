#pragma once
#include <wx/panel.h>

class cHighlightPanel : public wxPanel
{
public:
	cHighlightPanel(wxWindow* parent, 
		wxColour defaultColor,
		wxColour highlightColour,
		wxWindowID winid = wxID_ANY,
		wxPoint pos = wxDefaultPosition, 
		const wxSize size = wxDefaultSize);
	

	void SetSizerAndFit(wxSizer* sizer, bool deleteOld = true);

	// Binds the specified event to all component's children
	template <typename EventTag, typename Class, typename EventArg, typename EventHandler>
	void BindRecursively(
		wxWindow* component, 
		EventTag eventType,
		void (Class::* method)(EventArg&),
		EventHandler* handler);
private:

	wxColour colDefault;
	wxColour colHighlight;

	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
};

