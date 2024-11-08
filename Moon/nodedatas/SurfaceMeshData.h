#pragma once
#include <QtNodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
namespace MOON {
	class SurfaceMesh;
}


class SurfaceMeshData : public NodeData
{
public:
	SurfaceMeshData() {}

	SurfaceMeshData(const std::shared_ptr<MOON::SurfaceMesh>& srf)
		: _srf(srf)
	{}

	NodeDataType type() const override { return NodeDataType{ "surfacemesh", "SurfaceMesh" }; }

	std::shared_ptr<MOON::SurfaceMesh> mesh() const { return _srf; }

private:
	std::shared_ptr<MOON::SurfaceMesh> _srf;
};
