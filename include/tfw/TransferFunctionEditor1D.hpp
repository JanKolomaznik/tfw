#ifndef TRANSFERFUNCTIONEDITOR1D_HPP
#define TRANSFERFUNCTIONEDITOR1D_HPP

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class TransferFunctionEditor1D;
}

namespace tfw {


class TransferFunctionEditor1D : public QWidget
{
	Q_OBJECT
public:
	explicit TransferFunctionEditor1D(QWidget *parent = 0);
	~TransferFunctionEditor1D();

private:
	Ui::TransferFunctionEditor1D *ui;
	QGraphicsScene mTFScene;
};

} // namespace tfw

#endif // TRANSFERFUNCTIONEDITOR1D_HPP
