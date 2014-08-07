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

	QObject::connect(
		mUi->mPaletteView,
		&QListView::clicked,
		[this](const QModelIndex &index) {
			emit changedTransferFunctionSelection(index.row());
		});
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

int
PaletteWidget::getSelectedTransferFunctionIndex() const
{
	return mUi->mPaletteView->currentIndex().row();
}

const ATransferFunction &
PaletteWidget::getTransferFunction(int aIndex) const
{
	return mModel->palette().get(aIndex);
}

void PaletteWidget::onItemDoubleClicked(const QModelIndex &index)
{
	int tfId = index.row();
	auto &transfer_function = mModel->palette().get(tfId);

	auto it = mEditors.find(tfId);
	if (it == end(mEditors)) {
		auto visitor = CreateEditorVisitor();
		transfer_function.accept(visitor);
		mEditors[tfId] = { visitor.editor, visitor.editor };
		visitor.editor->setStatistics(mStatistics);
		QObject::connect(
			visitor.editor,
			&ATransferFunctionEditor::transferFunctionModified,
			[this, index] () {
				emit transferFunctionModified(index.row());
			});
	}
}

} // namespace tfw
