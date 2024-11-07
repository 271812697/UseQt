#pragma once
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QtNodes/DataFlowGraphModel>
namespace MOON {


	class Nodepanel : public QWidget
	{
	public:
		Nodepanel(QWidget* parent);
	private:
		std::unique_ptr<QtNodes::DataFlowGraphModel>dataFlowGraphModel;

	};
}