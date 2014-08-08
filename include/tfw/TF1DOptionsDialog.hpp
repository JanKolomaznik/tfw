#pragma once

#include <QDialog>

#include <memory>

#include "tfw/data/ATransferFunction.hpp"

namespace Ui {
class TF1DOptionsDialog;
}

namespace tfw {

class TF1DOptionsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TF1DOptionsDialog(QWidget *parent = nullptr);
	~TF1DOptionsDialog();

	std::string
	tfName() const;

	void
	setTFName(const std::string &aName);

	void
	setRange(TransferFunction1D::Range aRange);

	TransferFunction1D::Range
	range() const;
private:
	std::unique_ptr<Ui::TF1DOptionsDialog> mUi;
};

} // namespace tfw
