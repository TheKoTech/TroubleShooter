#include "ChartController.h"

ChartController::ChartController()
{


}

ChartController::~ChartController()
{
}

// Creates a serie and updates dataset
void ChartController::CreateSerie(SerieType serieType, wxVector<wxRealPoint> data)
{
	XYSerie* serie = new XYSerie(data);
	dataset->AddSerie(serie);
	dataset->DatasetChanged();
}

void ChartController::UpdateSerie(int id, wxRealPoint* newPoint = nullptr)
{
	
	dataset->DatasetChanged();
}

void ChartController::ClearDataset()
{
	dataset = nullptr;
	plot = nullptr;
	chart = nullptr;
}

XYPlot* ChartController::GetPlot()
{
	return plot;
}

XYSimpleDataset* ChartController::GetDataset()
{
	return dataset;
}
XYSerie* ChartController::GetSerie(int index)
{
	return dataset->GetSerie(index);
}

Chart* ChartController::CreateChart()
{
	dataset = new XYSimpleDataset();
	XYLineRenderer* renderer = new XYLineRenderer();
	dataset->SetRenderer(renderer);
	plot = new XYPlot();

	//// XY data for series
	wxVector<wxRealPoint> data;
	data.push_back(wxRealPoint(1, 1));

	// Third step: add a series to it.
	dataset->AddSerie(new XYSerie(data));

	// set line renderer to dataset
	dataset->SetRenderer(new XYLineRenderer());

	// create left and bottom number axes
	NumberAxis* rightAxis = new NumberAxis(AXIS_RIGHT);
	NumberAxis* bottomAxis = new NumberAxis(AXIS_BOTTOM);
	rightAxis->SetFixedBounds(0, 2000);
	bottomAxis->SetFixedBounds(0, 60);
	rightAxis->IntegerValues(true);
	bottomAxis->IntegerValues(true);

	// add axes and dataset to plot
	plot->AddObjects(dataset, rightAxis, bottomAxis);



	// and finally create chart
	return new Chart(plot, wxEmptyString);
}

