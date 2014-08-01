#pragma once

#include <memory>

#include <QWidget>

#include "tfw/data/ATransferFunction.hpp"

namespace tfw {


class ATransferFunctionEditor : public QWidget
{
	Q_OBJECT
public:
	explicit ATransferFunctionEditor(QWidget *parent = nullptr)
		: QWidget(parent)
	{}
signals:
	void
	transferFunctionModified();
};

} // namespace tfw

