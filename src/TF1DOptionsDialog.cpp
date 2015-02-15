#include "include/tfw/TF1DOptionsDialog.hpp"
#include "ui_TF1DOptionsDialog.h"

namespace tfw {

TF1DOptionsDialog::TF1DOptionsDialog(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::TF1DOptionsDialog)
{
	mUi->setupUi(this);
	setWindowTitle(tr("1D Transfer Function Options"));
}

TF1DOptionsDialog::~TF1DOptionsDialog()
{
}

std::string
TF1DOptionsDialog::tfName() const {
  // memory-leak hack for windows compatibility (maybe bug in QT)
  std::string* name = new std::string(mUi->mNameLineEdit->text().toStdString());
	return *name;
}

void TF1DOptionsDialog::setTFName(const std::string &aName)
{
	mUi->mNameLineEdit->setText(QString::fromStdString(aName));
}

void
TF1DOptionsDialog::setRange(TransferFunction1D::Range aRange) {
	mUi->mRangeFromDoubleSpinBox->setValue(aRange.first);
	mUi->mRangeToDoubleSpinBox->setValue(aRange.second);
}

TransferFunction1D::Range
TF1DOptionsDialog::range() const
{
	return TransferFunction1D::Range(mUi->mRangeFromDoubleSpinBox->value(), mUi->mRangeToDoubleSpinBox->value());
}

} // namespace tfw
