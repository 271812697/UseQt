#pragma once
#include <Eigen/Dense>
template <typename L, typename S, typename H, typename T>
void colon(
	const L low,
	const S step,
	const H hi,
	Eigen::Matrix<T, Eigen::Dynamic, 1>& I);

template <typename L, typename H, typename T>
void colon(
	const L low,
	const H hi,
	Eigen::Matrix<T, Eigen::Dynamic, 1>& I);

template <typename T, typename L, typename H>
Eigen::Matrix<T, Eigen::Dynamic, 1> colon(
	const L low,
	const H hi);

