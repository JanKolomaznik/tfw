#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>

#include "tfw/Exceptions.hpp"

namespace tfw {

class AStatistics
{
public:
	virtual bool
	hasHistogram() const = 0;

	virtual std::pair<float, float>
	getHistogramRange() const = 0;

	virtual std::vector<QPointF>
	getHistogramSamples() const = 0;
};

} // namespace tfw
