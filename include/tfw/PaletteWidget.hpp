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

public slots:
	void
	onItemDoubleClicked(const QModelIndex & index);
private:
	std::unique_ptr<Ui::PaletteWidget> mUi;
	std::unique_ptr<TransferFunctionPaletteModel> mModel;
};

} // namespace tfw

#endif // PALETTEWIDGET_HPP
