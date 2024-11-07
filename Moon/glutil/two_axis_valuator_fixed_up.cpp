#include "two_axis_valuator_fixed_up.h"
namespace MOON {
	inline constexpr double PI = 3.1415926535897932384626433832795;

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
		Eigen::Quaternion<Scalarquat>& quat)
	{
		Eigen::Matrix<Scalarquat, 3, 1> axis(0, 1, 0);
		quat = down_quat *
			Eigen::Quaternion<Scalarquat>(
				Eigen::AngleAxis<Scalarquat>(
					PI * ((Scalarquat)(mouse_x - down_x)) / (Scalarquat)w * speed / 2.0,
					axis.normalized()));
		quat.normalize();
		{
			Eigen::Matrix<Scalarquat, 3, 1> axis(1, 0, 0);
			if (axis.norm() != 0)
			{
				quat = Eigen::Quaternion<Scalarquat>(
					Eigen::AngleAxis<Scalarquat>(
						PI * (mouse_y - down_y) / (Scalarquat)h * speed / 2.0,
						axis.normalized())) * quat;
				quat.normalize();
			}
		}
	}


	template void two_axis_valuator_fixed_up<float, float>(int, int, double, Eigen::Quaternion<float, 0> const&, int, int, int, int, Eigen::Quaternion<float, 0>&);
	template void two_axis_valuator_fixed_up<double, double>(int, int, double, Eigen::Quaternion<double, 0> const&, int, int, int, int, Eigen::Quaternion<double, 0>&);


}
