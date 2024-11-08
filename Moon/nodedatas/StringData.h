#pragma once
#include <QtNodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class StringData : public NodeData
{
public:
	StringData() {}

	StringData(QString const& str)
		: _str(str)
	{}

	NodeDataType type() const override { return NodeDataType{ "sstring", "Sstring" }; }

	QString str() const { return _str; }

private:
	QString _str;
};
