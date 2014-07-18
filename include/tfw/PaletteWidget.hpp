#ifndef PALETTEWIDGET_HPP
#define PALETTEWIDGET_HPP

#include <memory>

#include <QWidget>

#include "tfw/data/TransferFunctionPalette.hpp"

namespace Ui {
class PaletteWidget;
} // namespace Ui

namespace tfw {

class PaletteWidget : public QWidget
{
	Q_OBJECT

public:
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
public slots:
	void
	onItemDoubleClicked(const QModelIndex & index);

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
};

} // namespace tfw

#endif // PALETTEWIDGET_HPP
