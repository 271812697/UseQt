#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>
namespace MOON {
	template <typename Scalardown_quat, typename Scalarquat>
	void two_axis_valuator_fixed_up(
		const int w,
		const int h,
		const double speed,
		const Eigen::Quaternion<Scalardown_quat>& down_quat,
		const int down_x,
		const int down_y,
		const int mouse_x,
		const int mouse_y,
		Eigen::Quaternion<Scalarquat>& quat);

}
