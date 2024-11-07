#pragma once
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>

namespace MOON {
	class Hierarchypanel : public QWidget
	{
	public:
		Hierarchypanel(QWidget* parent);
		QWidget* up_panel = nullptr;
		QVBoxLayout* up_panel_layout = nullptr;
	private:
		QPushButton* create = nullptr;
		QPushButton* remove = nullptr;
		QPushButton* rename = nullptr;
		QPushButton* import_ = nullptr;

		QListWidget* name_list = nullptr;
	};
}