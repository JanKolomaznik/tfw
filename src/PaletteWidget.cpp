#include "tfw/PaletteWidget.hpp"

#include <string>
#include <utility>
#include <vector>
#include <functional>

#include "ui_PaletteWidget.h"

#include "tfw/CreateEditorVisitor.hpp"
#include "tfw/ShowTFOptionsVisitor.hpp"

namespace tfw {

typedef std::pair< std::string, std::function<std::shared_ptr<ATransferFunction>()>> TFCreatorDescription;
// TODO - wrap in better design
std::vector<TFCreatorDescription> gTFCreators {
	TFCreatorDescription(
		"1D RGBA transfer function",
		[] () {
			return std::make_shared<TransferFunction1D>();
		}),
	TFCreatorDescription(
		"2D RGBA transfer function",
		[] () {
			return std::make_shared<TransferFunction2D>();
		})

};

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

	QObject::connect(
		mUi->mAddTFButton,
		&QPushButton::clicked,
		this,
		&PaletteWidget::onAddClicked);


	for (const auto &tfCreator : gTFCreators) {
		mUi->mTFTypeComboBox->addItem(tfCreator.first.c_str());
	}
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

void PaletteWidget::selectTransferFunction(int aTFId)
{
	mUi->mPaletteView->setCurrentIndex(mModel->modelIndexFromId(aTFId));
}

void PaletteWidget::onItemDoubleClicked(const QModelIndex &index)
{
	int tfId = index.row();
	auto &transfer_function = mModel->palette().get(tfId);

	auto it = mEditors.find(tfId);
	if (it == end(mEditors)) {
		auto visitor = CreateEditorVisitor();
		transfer_function.accept(visitor);
		QWidget *container = visitor.editor;
		if (mWrapEditorCallback) {
			container = mWrapEditorCallback(visitor.editor);
		}
		mEditors[tfId] = { visitor.editor, container };
		visitor.editor->setStatistics(mStatistics);
		QObject::connect(
			visitor.editor,
			&ATransferFunctionEditor::transferFunctionModified,
			[this, index] () {
				emit transferFunctionModified(index.row());
			});
	} else {
		it->second.container->show();
	}
}

void PaletteWidget::onAddClicked()
{
	auto newTF = gTFCreators.at(mUi->mTFTypeComboBox->currentIndex()).second();

	ShowTFOptionsVisitor showOptionsVisitor;
	newTF->accept(showOptionsVisitor);
	if (showOptionsVisitor.succeeded) {
		mModel->add(newTF);
	}
}

} // namespace tfw
