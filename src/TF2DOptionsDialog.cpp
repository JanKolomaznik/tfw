#include "include/tfw/TF2DOptionsDialog.hpp"
#include "ui_TF2DOptionsDialog.h"

namespace tfw {

TF2DOptionsDialog::TF2DOptionsDialog(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::TF2DOptionsDialog)
{
	mUi->setupUi(this);
	setWindowTitle(tr("2D Transfer Function Options"));
}

TF2DOptionsDialog::~TF2DOptionsDialog()
{
}

std::string
TF2DOptionsDialog::tfName() const {
	return mUi->mNameLineEdit->text().toStdString();
}

void TF2DOptionsDialog::setTFName(const std::string &aName)
{
	mUi->mNameLineEdit->setText(QString::fromStdString(aName));
}

void
TF2DOptionsDialog::setRange(TransferFunction2D::Range aRange) {
	mUi->mRangeXFromDoubleSpinBox->setValue(aRange.first[0]);
	mUi->mRangeXToDoubleSpinBox->setValue(aRange.second[0]);
	mUi->mRangeYFromDoubleSpinBox->setValue(aRange.first[1]);
	mUi->mRangeYToDoubleSpinBox->setValue(aRange.second[1]);
}

TransferFunction2D::Range
TF2DOptionsDialog::range() const
{
	return TransferFunction2D::Range(
		TransferFunction2D::RangePoint{mUi->mRangeXFromDoubleSpinBox->value(), mUi->mRangeYFromDoubleSpinBox->value()},
		TransferFunction2D::RangePoint{mUi->mRangeXToDoubleSpinBox->value(), mUi->mRangeYToDoubleSpinBox->value()});
}

} // namespace tfw
