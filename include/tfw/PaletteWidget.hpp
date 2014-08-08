#ifndef PALETTEWIDGET_HPP
#define PALETTEWIDGET_HPP

#include <memory>
#include <map>

#include <QWidget>

#include "tfw/ATransferFunctionEditor.hpp"
#include "tfw/data/TransferFunctionPalette.hpp"
#include "tfw/data/AStatistics.hpp"

namespace Ui {
class PaletteWidget;
} // namespace Ui

namespace tfw {




class PaletteWidget : public QWidget
{
	Q_OBJECT
public:
	typedef std::function<QWidget *(tfw::ATransferFunctionEditor *)> WrapEditorCallback;

	explicit PaletteWidget(QWidget *parent = nullptr);
	~PaletteWidget();

	void
	setPalette(std::shared_ptr<TransferFunctionPalette> aPalette);

	int
	getSelectedTransferFunctionIndex() const;

	const ATransferFunction &
	getTransferFunction(int aIndex) const;

	const ATransferFunction &
	getSelectedTransferFunction() const {
		return getTransferFunction(getSelectedTransferFunctionIndex());
	}

	void
	setStatistics(std::shared_ptr<AStatistics> aStatistics) {
		mStatistics = aStatistics;
		for (auto &editor : mEditors) {
			editor.second.editor->setStatistics(mStatistics);
		}
	}

	void
	setWrapEditorCallback(WrapEditorCallback aCallback)
	{
		mWrapEditorCallback = std::move(aCallback);
	}

	void
	selectTransferFunction(int aTFId);

public slots:
	void
	onItemDoubleClicked(const QModelIndex & index);

	void
	onAddClicked();

signals:
	void
	transferFunctionAdded(int idx);
	void
	transferFunctionModified(int idx);
	void
	changedTransferFunctionSelection(int index);
private:
	std::unique_ptr<Ui::PaletteWidget> mUi;
	std::unique_ptr<TransferFunctionPaletteModel> mModel;

	struct EditorRecord {
		ATransferFunctionEditor *editor;
		QWidget *container;
	};


	std::map<int, EditorRecord> mEditors;

	std::shared_ptr<AStatistics> mStatistics;

	WrapEditorCallback mWrapEditorCallback;
};

} // namespace tfw

#endif // PALETTEWIDGET_HPP
