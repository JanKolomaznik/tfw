#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>

#include <QPointF>
#include <QRectF>

#include "tfw/Exceptions.hpp"

namespace tfw {

struct ScatterPlotData
{
	std::array<int, 2> size;
	std::vector<float> buffer;
};

class AStatistics
{
public:
	virtual bool
	hasHistogram() const = 0;

	virtual std::pair<float, float>
	getHistogramRange() const = 0;

	virtual std::vector<QPointF>
	getHistogramSamples() const = 0;

	virtual bool
	hasScatterPlot() const = 0;

	virtual std::pair<QRectF, ScatterPlotData>
	getScatterPlot() const = 0;

};

} // namespace tfw
