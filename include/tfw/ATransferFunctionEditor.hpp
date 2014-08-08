#pragma once

#include <memory>

#include <QWidget>

#include "tfw/data/ATransferFunction.hpp"
#include "tfw/data/AStatistics.hpp"

namespace tfw {


class ATransferFunctionEditor : public QWidget
{
	Q_OBJECT
public:
	explicit ATransferFunctionEditor(QWidget *parent = nullptr)
		: QWidget(parent)
	{}

	virtual QString
	tfName() const = 0;

	virtual void
	setStatistics(std::shared_ptr<AStatistics> aStatistics) = 0;
signals:
	void
	transferFunctionModified();
};

} // namespace tfw

