#pragma once

#include <memory>
#include <string>

//TODO - move to utils
#include <stdexcept>
#include <exception>
#include <boost/exception/all.hpp>

#define TFW_THROW(x) BOOST_THROW_EXCEPTION(x)

namespace tfw {

class TFWException: public virtual boost::exception, public virtual std::exception {};

} // namesapce tfw

namespace tfw {

class ITransferFunctionVisitor
{
public:
	virtual
	~ITransferFunctionVisitor() {}

};

class ATransferFunction
{
public:
	virtual void
	accept(ITransferFunctionVisitor &aVisitor) = 0;


	std::string
	name() const
	{
		return "Dummy";
	}
};

} // namesapce tfw
