#pragma once

#include <QDialog>

#include <memory>

#include "tfw/data/ATransferFunction.hpp"

namespace Ui {
class TF2DOptionsDialog;
}

namespace tfw {

class TF2DOptionsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TF2DOptionsDialog(QWidget *parent = nullptr);
	~TF2DOptionsDialog();

	std::string
	tfName() const;

	void
	setTFName(const std::string &aName);

	void
	setRange(TransferFunction2D::Range aRange);

	TransferFunction2D::Range
	range() const;

	void
	setSamplingRate(TransferFunction2D::SamplingRate aSamplingRate);

	TransferFunction2D::SamplingRate
	samplingRate() const;
private:
	std::unique_ptr<Ui::TF2DOptionsDialog> mUi;
};

} // namespace tfw
