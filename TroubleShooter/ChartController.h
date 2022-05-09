#pragma once
#include "wx/plot.h"
#include <wx/xy/xysimpledataset.h>
#include <wx/xy/xyplot.h>
#include <wx/xy/xylinerenderer.h>
#include <wx/axisplot.h>


/*
This controller class stores data (Series) for displaying on the frame.
The data is taken from the log file, then managed and saved here as Series.
Each serie belongs to one host that is being pinged.
They're saved here and referenced outside the class by pointers.
*/
class ChartController
{
public:
	enum SerieType { dsLAN, dsISP, dsDNS, dsDomain };
	
	ChartController();
	~ChartController();

	//This function creates a dataset to be shown
	void CreateSerie(SerieType type, wxVector<wxRealPoint> data);
	// Returns a dataset so it can be shown
	void UpdateSerie(int id, wxRealPoint* newPoint);
	void ClearDataset();
	
	XYPlot* GetPlot();
	XYSimpleDataset* GetDataset();
	XYSerie* GetSerie(int index);

	Chart* CreateChart();

private:
	XYSimpleDataset* dataset;
	XYPlot* plot;
	Chart* chart;

};
