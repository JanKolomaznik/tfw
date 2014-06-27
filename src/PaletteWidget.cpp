#include "tfw/PaletteWidget.hpp"

#include "ui_PaletteWidget.h"

#include "tfw/CreateEditorVisitor.hpp"

namespace tfw {

PaletteWidget::PaletteWidget(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::PaletteWidget())
{
	mUi->setupUi(this);

	QObject::connect(mUi->mPaletteView, &QListView::doubleClicked, this, &PaletteWidget::onItemDoubleClicked);
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

void PaletteWidget::onItemDoubleClicked(const QModelIndex &index)
{
	const auto &transfer_function = mModel->palette().get(index.row());
	auto visitor = CreateEditorVisitor();
	transfer_function.accept(visitor);
}

} // namespace tfw
