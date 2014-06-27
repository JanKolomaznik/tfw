#pragma once

#include "tfw/data/ATransferFunction.hpp"
#include "tfw/ATransferFunctionEditor.hpp"

#include "tfw/TransferFunctionEditor1D.hpp"

namespace tfw {

class CreateEditorVisitor : public ITransferFunctionVisitor
{
public:
	virtual void
	visit(const TransferFunction1D &) override
	{
		// TODO
		auto editor = new TransferFunctionEditor1D();
		editor->show();
	}

private:

};

} // namespace tfw

