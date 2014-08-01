#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>

#include "tfw/Exceptions.hpp"

namespace tfw {


} // namesapce tfw

namespace tfw {

class TransferFunction1D;



class ITransferFunctionVisitor
{
public:
	virtual
	~ITransferFunctionVisitor() {}

	virtual void
	visit(TransferFunction1D &aTF) = 0;
};

class IConstTransferFunctionVisitor
{
public:
	virtual
	~IConstTransferFunctionVisitor() {}

	virtual void
	visit(const TransferFunction1D &) = 0;
};



class UnsupportedThrowTransferFunctionVisitor : public ITransferFunctionVisitor
{
public:
	void
	visit(TransferFunction1D &) override
	{
		TFW_THROW(EUnsupportedTransferFunctionType()); // TODO - message
	}
};

class UnsupportedThrowConstTransferFunctionVisitor : public IConstTransferFunctionVisitor
{
public:
	void
	visit(const TransferFunction1D &) override
	{
		TFW_THROW(EUnsupportedTransferFunctionType()); // TODO - message
	}
};


class ATransferFunction
{
public:
	virtual void
	accept(ITransferFunctionVisitor &aVisitor) = 0;

	virtual void
	accept(IConstTransferFunctionVisitor &aVisitor) const = 0;

	std::string
	name() const
	{
		return "Dummy";
	}
};


template<typename TContainer, typename TPoint, typename TComparator>
void
insertRange(TContainer &aContainer, TPoint aFirst, TPoint aSecond, TComparator aComparator)
{
	if (!aComparator(aFirst, aSecond)) {
		std::swap(aFirst, aSecond);
	}
	auto lower = std::lower_bound(begin(aContainer), end(aContainer), aFirst, aComparator);
	auto upper = std::upper_bound(lower, end(aContainer), aSecond, aComparator);

	auto it = aContainer.erase(lower, upper);
	std::array<TPoint, 2> tmpVec = { aFirst, aSecond };
	aContainer.insert(it, begin(tmpVec), end(tmpVec));
}


class TransferFunction1D : public ATransferFunction
{
public:
	typedef float RangePoint;
	typedef float Intensity;
	typedef std::pair<RangePoint, Intensity> Point;
	typedef std::pair<RangePoint, RangePoint> Range;

	TransferFunction1D(RangePoint aFrom = 0.0, RangePoint aTo = 1.0)
		: mRange(aFrom, aTo)
	{
	}

	static bool pointCompare(const Point &a, const Point &b)
	{
		return a.first < b.first;
	}

	void
	accept(ITransferFunctionVisitor &aVisitor) override
	{
		aVisitor.visit(*this);
	}

	void
	accept(IConstTransferFunctionVisitor &aVisitor) const override
	{
		aVisitor.visit(*this);
	}

	void
	setChannelValue(RangePoint aValue, Intensity aIntensity, int aChannel)
	{
		TFW_ASSERT(aChannel >= 0 && aChannel < 4);
		Point newPoint(aValue, aIntensity);
		auto it = std::upper_bound(
			begin(mChannels[aChannel]),
			end(mChannels[aChannel]),
			newPoint,
			&pointCompare);
		mChannels[aChannel].insert(it, newPoint);
	}

	void
	setChannelRange(RangePoint aFrom, Intensity aFromIntensity, RangePoint aTo, Intensity aToIntensity, int aChannel)
	{
		// TODO - interpolate and clamp to range
		Point newPoint1(aFrom, aFromIntensity);
		Point newPoint2(aTo, aToIntensity);
		tfw::insertRange(mChannels[aChannel], newPoint1, newPoint2, &pointCompare);
	}

	Intensity
	getIntensity(RangePoint aValue, int aChannel) const
	{
		const std::vector<Point> &channel = mChannels[aChannel];
		if (channel.empty()) {
			return 0;
		}
		if (channel.front().first > aValue || channel.back().first < aValue) {
			return 0;
		}
		if (channel.size() == 2) {
			return ((aValue - channel[0].first) * channel[0].second + (channel[1].first - aValue) * channel[1].second)
				/ (channel[1].first - channel[0].first);
		}
		auto right = std::lower_bound(++begin(channel), end(channel), Point(aValue, 0), &pointCompare);
		auto left = right - 1;
		return (left->second * (aValue - left->first) + right->second * (right->first - aValue)) / (right->first - left->first);
	}

	const Range &
	range() const
	{
		return mRange;
	}

	void
	setRange(Range aRange)
	{
		mRange = aRange;
	}

protected:
	std::array<std::vector<Point>, 4> mChannels;
	std::pair<RangePoint, RangePoint> mRange;
};

} // namesapce tfw
