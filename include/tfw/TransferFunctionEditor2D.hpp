#pragma once

#include <array>

#include <QWidget>
#include <QGraphicsScene>
#include <QRadioButton>

#include "tfw/TransferFunctionView.hpp"
#include "tfw/ATransferFunctionEditor.hpp"
#include "tfw/data/AStatistics.hpp"
#include "tfw/GraphicsItems.hpp"

namespace Ui {
class TransferFunctionEditor2D;
}

namespace tfw {


class TransferFunctionEditor2D : public ATransferFunctionEditor
{
	Q_OBJECT
public:
	static const int cChannelCount = 4;

	explicit TransferFunctionEditor2D(QWidget *parent = nullptr);
	~TransferFunctionEditor2D();

	void
	setTransferFunction(TransferFunction1D &aTransferFunction);

	void
	setStatistics(std::shared_ptr<AStatistics> aStatistics) override;
private:
	Ui::TransferFunctionEditor2D *ui;
	QGraphicsScene mTFScene;
	std::shared_ptr<AStatistics> mStatistics;
};

} // namespace tfw

