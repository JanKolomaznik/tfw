#pragma once

#include <memory>
#include <string>

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
	visit(TransferFunction1D &aTF)
	{
		visit(static_cast<const TransferFunction1D &>(aTF));
	}

	virtual void
	visit(const TransferFunction1D &) = 0;
};

class ATransferFunction
{
public:
	virtual void
	accept(ITransferFunctionVisitor &aVisitor) = 0;

	virtual void
	accept(ITransferFunctionVisitor &aVisitor) const = 0;


	std::string
	name() const
	{
		return "Dummy";
	}
};


class TransferFunction1D : public ATransferFunction
{
public:
	void
	accept(ITransferFunctionVisitor &aVisitor) override
	{
		aVisitor.visit(*this);
	}

	void
	accept(ITransferFunctionVisitor &aVisitor) const override
	{
		aVisitor.visit(*this);
	}


};

} // namesapce tfw
