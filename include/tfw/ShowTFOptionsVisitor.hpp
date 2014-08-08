#pragma once

#include "tfw/data/ATransferFunction.hpp"

#include "tfw/TF1DOptionsDialog.hpp"

#include <QApplication>

namespace tfw {

class ShowTFOptionsVisitor : public UnsupportedThrowTransferFunctionVisitor
{
public:
	ShowTFOptionsVisitor()
		: succeeded(false)
	{}

	virtual void
	visit(TransferFunction1D &aTransferFunction) override
	{
		TF1DOptionsDialog dialog;
		dialog.setTFName(aTransferFunction.name());
		dialog.setRange(aTransferFunction.range());

		succeeded = dialog.exec() == QDialog::Accepted;
		if (succeeded) {
			aTransferFunction.setName(dialog.tfName());
			aTransferFunction.setRange(dialog.range());
		}
	}

	bool succeeded;
};

} // namespace tfw

