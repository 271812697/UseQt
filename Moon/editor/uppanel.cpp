#include <QVBoxLayout>
#include "uppanel.h"
#include "viewerwindow.h"
#include <QPushButton>
namespace MOON {



	UpPanel::UpPanel(QWidget* parent, Qt::WindowFlags f) :QWidget(parent)
	{
		auto preview_window_ = new ViewerWindow(this);
		QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);

		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->addWidget(preview_window_);

	}

}