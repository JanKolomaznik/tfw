#include "tfw/PaletteWidget.hpp"

#include "ui_PaletteWidget.h"

namespace tfw {

PaletteWidget::PaletteWidget(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::PaletteWidget())
{
	mUi->setupUi(this);
}

PaletteWidget::~PaletteWidget()
{
}

void
PaletteWidget::setPalette(std::shared_ptr<TransferFunctionPalette> aPalette)
{
	mModel.reset(new TransferFunctionPaletteModel(aPalette));
	mUi->mPaletteView->setModel(mModel.get());
}

} // namespace tfw
