#pragma once

#include "tfw/data/ATransferFunction.hpp"
#include "tfw/ATransferFunctionEditor.hpp"

#include "tfw/TransferFunctionEditor1D.hpp"
#include "tfw/TransferFunctionEditor2D.hpp"

#include <QApplication>

namespace tfw {

class CreateEditorVisitor : public UnsupportedThrowTransferFunctionVisitor
{
public:
	void
	visit(TransferFunction1D &aTransferFunction) override
	{
		// TODO
		auto tf_editor = new TransferFunctionEditor1D(QApplication::activeWindow());
		tf_editor->setTransferFunction(aTransferFunction);
		editor = tf_editor;
		editor->show();
	}

	void
	visit(TransferFunction2D &aTransferFunction) override
	{
		// TODO
		auto tf_editor = new TransferFunctionEditor2D(QApplication::activeWindow());
		tf_editor->setTransferFunction(aTransferFunction);
		editor = tf_editor;
		editor->show();
	}

	ATransferFunctionEditor *editor;
};

} // namespace tfw

