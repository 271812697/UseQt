#pragma once
#include <QtNodes/NodeDelegateModel>
#include <QtCore/QObject>
#include <iostream>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class QLineEdit;
class QPushButton;
class StringData;
class SurfaceMeshData;
/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ReadMeshModel : public NodeDelegateModel
{
	Q_OBJECT

public:
	ReadMeshModel();

	virtual ~ReadMeshModel() {}

public:
	QString caption() const override { return QStringLiteral("Read Mesh"); }

	bool captionVisible() const override { return false; }

	QString name() const override { return QStringLiteral("MeshLoader"); }

public:
	QJsonObject save() const override;

	void load(QJsonObject const& p) override;

public:
	unsigned int nPorts(PortType portType) const override;

	NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

	std::shared_ptr<NodeData> outData(PortIndex port) override;

	void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

	QWidget* embeddedWidget() override;

public:
	void setPath(QString const& path);

private Q_SLOTS:

	void onPathEdited(QString const& string);
	void onShow();
Q_SIGNALS:
	void viewnode(const std::shared_ptr<NodeData>& node);

private:
	std::shared_ptr<StringData> _path;
	std::shared_ptr<SurfaceMeshData> _srf;
	QWidget* _param;
	QPushButton* _show;
	QLineEdit* _lineEdit;
};
