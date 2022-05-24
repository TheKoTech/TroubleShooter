#pragma once
#include "wx/plot.h"
#include <wx/xy/xysimpledataset.h>
#include <wx/xy/xyplot.h>
#include <wx/xy/xylinerenderer.h>
#include <wx/axisplot.h>


// Этот класс-контроллер хранит данные в Сериях (Series) для отображения на cFrame.
// Эти данные собираются в cApp из файла лога, затем обрабатываются и сохраняются здесь как Series.
// Каждая Serie относится к одному SerieType, который пингует приложение.
// Этот класс предоставляет метод CreateChart(), который возвращает Chart с указателями на данные, которые могут быть изменены отсюда.

/*
This controller class stores data (Series) for displaying on the cFrame.
The data is taken from the log file in cApp, then managed and saved here as Series.
Each serie belongs to one SerieType that is being pinged.
The class provides a CreateChart() method that returns a chart with pointers to the data that can be changed from here.
*/
class ChartController
{
public:
	// The 4 types of IP addressList that are being pinged. Each represents one chart serie.
	enum SerieType { dsLAN = 0,
		dsISP = 1, 
		dsDNS = 2, 
		dsDomain = 3
	};
	
	ChartController();
	~ChartController();


	// Creates a serie and updates dataset
	// @param type - type of serie (LAN, ISP, DNS...).
	// @param data - a vector of data to be shown.
	void CreateSerie(SerieType type, wxVector<wxRealPoint> data);
	

	// [NOT TESTED] Appends a new point to a serie and shifts the other points to the left
	// @param type - type of serie (LAN, ISP, DNS...)
	// @param newX - the new Y value that will be appended to the 0sec X value
	void AppendToSerie(SerieType serieType, double newY);


	//Clears dataset (use in cApp)
	void ClearDataset();
	
	// Getters

	//XYPlot* GetPlot();
	//XYSimpleDataset* GetDataset();
	//XYSerie* GetSerie(int index);
	//XYSerie* GetSerie(SerieType type);

	//UI
	
	//Creates a chart, links the dataset with it to manage its contents.
	Chart* CreateChart();

private:
	XYSimpleDataset* dataset;
	XYPlot* plot;
	Chart* chart;

};
