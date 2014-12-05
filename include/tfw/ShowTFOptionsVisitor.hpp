#pragma once

#include "tfw/data/ATransferFunction.hpp"

#include "tfw/TF1DOptionsDialog.hpp"
#include "tfw/TF2DOptionsDialog.hpp"

#include <QApplication>

namespace tfw {

class ShowTFOptionsVisitor : public UnsupportedThrowTransferFunctionVisitor
{
public:
	ShowTFOptionsVisitor()
		: succeeded(false)
	{}

	void
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

	void
	visit(TransferFunction2D &aTransferFunction) override
	{
		TF2DOptionsDialog dialog;
		dialog.setTFName(aTransferFunction.name());
		dialog.setRange(aTransferFunction.range());
		dialog.setSamplingRate(aTransferFunction.samplingRate());

		succeeded = dialog.exec() == QDialog::Accepted;
		if (succeeded) {
			aTransferFunction.setName(dialog.tfName());
			aTransferFunction.setRange(dialog.range());
			aTransferFunction.setSamplingRate(dialog.samplingRate());
		}
	}

	bool succeeded;
};

} // namespace tfw

