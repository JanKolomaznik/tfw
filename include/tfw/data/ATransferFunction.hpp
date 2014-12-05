#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>

#include "tfw/Exceptions.hpp"

#include <boost/multi_array.hpp>

namespace tfw {


} // namesapce tfw

namespace tfw {

class TransferFunction1D;
class TransferFunction2D;



class ITransferFunctionVisitor
{
public:
	virtual
	~ITransferFunctionVisitor() {}

	virtual void
	visit(TransferFunction1D &aTF) = 0;

	virtual void
	visit(TransferFunction2D &aTF) = 0;
};

class IConstTransferFunctionVisitor
{
public:
	virtual
	~IConstTransferFunctionVisitor() {}

	virtual void
	visit(const TransferFunction1D &) = 0;

	virtual void
	visit(const TransferFunction2D &) = 0;
};



class UnsupportedThrowTransferFunctionVisitor : public ITransferFunctionVisitor
{
public:
	void
	visit(TransferFunction1D &) override
	{
		TFW_THROW(EUnsupportedTransferFunctionType()); // TODO - message
	}

	void
	visit(TransferFunction2D &) override
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

	void
	visit(const TransferFunction2D &) override
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
		return mName;
	}

	void
	setName(std::string aName)
	{
		mName = std::move(aName);
	}
protected:
	std::string mName;
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
		// TODO - other interpolation options
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

	std::vector<Point>::const_iterator
	channelBegin(int aChannel) const
	{
		return mChannels[aChannel].begin();
	}

	std::vector<Point>::const_iterator
	channelEnd(int aChannel) const
	{
		return mChannels[aChannel].end();
	}

protected:
	std::array<std::vector<Point>, 4> mChannels;
	Range mRange;
};


class TransferFunction2D : public ATransferFunction
{
public:
	typedef float FloatingPoint;
	typedef std::array<float, 2> SamplingRate;
	typedef std::array<float, 2> RangePoint;
	typedef std::array<float, 4> Color;
	typedef std::vector<RangePoint> Polygon;
	typedef std::pair<RangePoint, RangePoint> Range;

	TransferFunction2D(
			RangePoint aFrom = RangePoint{ 0.0, 0.0 },
			RangePoint aTo = RangePoint{ 1.0, 1.0 },
			SamplingRate aSampleRate = { 1.0, 1.0})
		: mRange(aFrom, aTo)
		, mSampleRate(aSampleRate)
	{
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

	Color
	getColor(RangePoint aValue) const
	{
		return getColor(aValue[0], aValue[1]);
	}

	Color
	getColor(FloatingPoint aXValue, FloatingPoint aYValue) const
	{
		//return Color{ 0.0f, 0.5f, 1.0f, 0.5f };
		return mBuffer[int(aXValue)][int(aYValue)]; //TODO
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
		resize();
	}


	void
	setSamplingRate(SamplingRate aSamplingRate)
	{
		mSampleRate = aSamplingRate;
		resize();
	}

	SamplingRate
	samplingRate() const
	{
		return mSampleRate;
	}

	void
	setColor(FloatingPoint aXValue, FloatingPoint aYValue, Color aColor)
	{
		mBuffer[int(aXValue)][int(aYValue)] = aColor; //TODO
	}

	void
	setColor(RangePoint aFrom, RangePoint aTo, Color aColor)
	{
		int xFrom = std::max<int>(0, std::floor(aFrom[0] * mSampleRate[0]));
		int yFrom = std::max<int>(0, std::floor(aFrom[1] * mSampleRate[1]));
		int xTo = std::min<int>(mBuffer.shape()[0] - 1, std::ceil(aTo[0] * mSampleRate[0]));
		int yTo = std::min<int>(mBuffer.shape()[1] - 1, std::ceil(aTo[1] * mSampleRate[1]));
		/*int xFrom = std::floor(aFrom[0]);
		int yFrom = std::floor(aFrom[1]);
		int xTo = std::ceil(aTo[0]);
		int yTo = std::ceil(aTo[1]);*/
		for (int j = yFrom; j <= yTo; ++j) {
			for (int i = xFrom; i <= xTo; ++i) {
				mBuffer[i][j] = aColor;
			}
		}
		//TODO
	}

	void
	reset() {
		std::fill(mBuffer.origin() , mBuffer.origin() + mBuffer.size(), Color {0.0, 0.0, 0.0, 0.0});
		for (int j = 0; j < int(mBuffer.shape()[1]); ++j) {
			for (int i = 0; i < int(mBuffer.shape()[0]); ++i) {
				mBuffer[i][j] = Color {0.0, 0.0, 0.0, 0.0};
			}
		}
	}

protected:
	void
	resize()
	{
		std::array<int, 2> newExtents = {{
			int(mSampleRate[0] * (mRange.second[0] - mRange.first[0])),
			int(mSampleRate[1] * (mRange.second[1] - mRange.first[1]))
			}};
		mBuffer.resize(newExtents); //TODO
	}

	typedef boost::multi_array<Color, 2> Buffer;

	Buffer mBuffer;
	//std::vector<Polygon> mPolygons;
	//std::vector<std::pair<Range, Color>> mPolygons;

	Range mRange;
	SamplingRate mSampleRate;
};

} // namesapce tfw
