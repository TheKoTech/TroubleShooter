#include "ChartController.h"

#define MINUTE 59 // 59 since counting from zero

ChartController::ChartController() = default;
ChartController::~ChartController() = default;

void ChartController::CreateSerie(SerieType type, wxVector<wxRealPoint> data)
{
	if (0 <= type && type <= 3) {
		auto* serie = new XYSerie(data);
		dataset->AddSerie(serie);
		dataset->DatasetChanged();
	}
}

void ChartController::AppendToSerie(SerieType type, double newDelay)
{
	// shift values to the left
	for (int i = 0; i < MINUTE; i++)
		dataset->GetSerie(type)->UpdateX(i, dataset->GetSerie(type)->GetX(i + 1));

	// the new value is put at the 0 sec mark - the right edge of the chart
	dataset->GetSerie(type)->UpdatePoint(MINUTE, wxPoint(MINUTE, newDelay));

	dataset->DatasetChanged();
}

void ChartController::ClearDataset()
{
	dataset = nullptr;
	plot = nullptr;
	chart = nullptr;
}

//XYPlot* ChartController::GetPlot()
//{
//	return plot;
//}
//
//XYSimpleDataset* ChartController::GetDataset()
//{
//	return dataset;
//}
//XYSerie* ChartController::GetSerie(int index)
//{
//	return dataset->GetSerie(index);
//}
//XYSerie* ChartController::GetSerie(SerieType type)
//{
//	return dataset->GetSerie(type);
//}


Chart* ChartController::CreateChart()
{
	dataset = new XYSimpleDataset();
	auto* renderer = new XYLineRenderer();
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
	auto* rightAxis = new NumberAxis(AXIS_RIGHT);
	auto* bottomAxis = new NumberAxis(AXIS_BOTTOM);
	rightAxis->SetFixedBounds(0, 2000);
	bottomAxis->SetFixedBounds(0, 60);
	rightAxis->IntegerValues(true);
	bottomAxis->IntegerValues(true);

	// add axes and dataset to plot
	plot->AddObjects(dataset, rightAxis, bottomAxis);

	// and finally create chart
	return new Chart(plot, wxEmptyString);
}
