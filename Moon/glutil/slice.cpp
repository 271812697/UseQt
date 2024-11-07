#include "slice.h"
#include "colon.h"

#include <vector>

template <
	typename TX,
	typename TY,
	typename DerivedR,
	typename DerivedC>
void slice(
	const Eigen::SparseMatrix<TX>& X,
	const Eigen::DenseBase<DerivedR>& R,
	const Eigen::DenseBase<DerivedC>& C,
	Eigen::SparseMatrix<TY>& Y)
{
	int xm = X.rows();
	int xn = X.cols();
	int ym = R.size();
	int yn = C.size();

	// special case when R or C is empty
	if (ym == 0 || yn == 0)
	{
		Y.resize(ym, yn);
		return;
	}

	assert(R.minCoeff() >= 0);
	assert(R.maxCoeff() < xm);
	assert(C.minCoeff() >= 0);
	assert(C.maxCoeff() < xn);

	// Build reindexing maps for columns and rows
	std::vector<std::vector<typename DerivedR::Scalar>> RI;
	RI.resize(xm);
	for (int i = 0; i < ym; i++)
	{
		RI[R(i)].push_back(i);
	}
	std::vector<std::vector<typename DerivedC::Scalar>> CI;
	CI.resize(xn);
	for (int i = 0; i < yn; i++)
	{
		CI[C(i)].push_back(i);
	}

	// Take a guess at the number of nonzeros (this assumes uniform distribution
	// not banded or heavily diagonal)
	std::vector<Eigen::Triplet<TY>> entries;
	entries.reserve((X.nonZeros() / (X.rows() * X.cols())) * (ym * yn));

	// Iterate over outside
	for (int k = 0; k < X.outerSize(); ++k)
	{
		// Iterate over inside
		for (typename Eigen::SparseMatrix<TX>::InnerIterator it(X, k); it; ++it)
		{
			for (auto rit = RI[it.row()].begin(); rit != RI[it.row()].end(); rit++)
			{
				for (auto cit = CI[it.col()].begin(); cit != CI[it.col()].end(); cit++)
				{
					entries.emplace_back(*rit, *cit, it.value());
				}
			}
		}
	}
	Y.resize(ym, yn);
	Y.setFromTriplets(entries.begin(), entries.end());
}

template <typename MatX, typename DerivedR, typename MatY>
void slice(
	const MatX& X,
	const Eigen::DenseBase<DerivedR>& R,
	const int dim,
	MatY& Y)
{
	Eigen::Matrix<typename DerivedR::Scalar, Eigen::Dynamic, 1> C;
	switch (dim)
	{
	case 1:
		// boring base case
		if (X.cols() == 0)
		{
			Y.resize(R.size(), 0);
			return;
		}
		colon(0, X.cols() - 1, C);
		return slice(X, R, C, Y);
	case 2:
		// boring base case
		if (X.rows() == 0)
		{
			Y.resize(0, R.size());
			return;
		}
		colon(0, X.rows() - 1, C);
		return slice(X, C, R, Y);
	default:
		assert(false && "Unsupported dimension");
		return;
	}
}

template <
	typename DerivedX,
	typename DerivedR,
	typename DerivedC,
	typename DerivedY>
void slice(
	const Eigen::DenseBase<DerivedX>& X,
	const Eigen::DenseBase<DerivedR>& R,
	const Eigen::DenseBase<DerivedC>& C,
	Eigen::PlainObjectBase<DerivedY>& Y)
{
#ifndef NDEBUG
	int xm = X.rows();
	int xn = X.cols();
#endif
	int ym = R.size();
	int yn = C.size();

	// special case when R or C is empty
	if (ym == 0 || yn == 0)
	{
		Y.resize(ym, yn);
		return;
	}

	assert(R.minCoeff() >= 0);
	assert(R.maxCoeff() < xm);
	assert(C.minCoeff() >= 0);
	assert(C.maxCoeff() < xn);

	// Resize output
	Y.resize(ym, yn);
	// loop over output rows, then columns
	for (int i = 0; i < ym; i++)
	{
		for (int j = 0; j < yn; j++)
		{
			Y(i, j) = X(R(i), C(j));
		}
	}
}

template <typename DerivedX, typename DerivedY, typename DerivedR>
void slice(
	const Eigen::DenseBase<DerivedX>& X,
	const Eigen::DenseBase<DerivedR>& R,
	Eigen::PlainObjectBase<DerivedY>& Y)
{
	// phony column indices
	Eigen::Matrix<typename DerivedR::Scalar, Eigen::Dynamic, 1> C;
	C.resize(1);
	C(0) = 0;
	return slice(X, R, C, Y);
}

template <typename DerivedX, typename DerivedR>
DerivedX slice(
	const Eigen::DenseBase<DerivedX>& X,
	const Eigen::DenseBase<DerivedR>& R)
{
	DerivedX Y;
	slice(X, R, Y);
	return Y;
}

template <typename DerivedX, typename DerivedR>
DerivedX slice(
	const Eigen::DenseBase<DerivedX>& X,
	const Eigen::DenseBase<DerivedR>& R,
	const int dim)
{
	DerivedX Y;
	slice(X, R, dim, Y);
	return Y;
}

template< class T >
void slice(
	const std::vector<T>& unordered,
	std::vector<size_t> const& index_map,
	std::vector<T>& ordered)
{
	// copy for the slice according to index_map, because unordered may also be
	// ordered
	std::vector<T> copy = unordered;
	ordered.resize(index_map.size());
	for (int i = 0; i < (int)index_map.size(); i++)
	{
		ordered[i] = copy[index_map[i]];
	}
}


// Explicit template instantiation
template void slice<Eigen::SparseMatrix<bool, 0, int>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::SparseMatrix<bool, 0, int>>(Eigen::SparseMatrix<bool, 0, int> const&, Eigen::DenseBase<Eigen::Matrix<int, -1, 1, 0, -1, 1>> const&, int, Eigen::SparseMatrix<bool, 0, int>&);
template void slice<Eigen::SparseMatrix<double, 0, int>, Eigen::Array<int, -1, 1, 0, -1, 1>, Eigen::SparseMatrix<double, 0, int> >(Eigen::SparseMatrix<double, 0, int> const&, Eigen::DenseBase<Eigen::Array<int, -1, 1, 0, -1, 1> > const&, int, Eigen::SparseMatrix<double, 0, int>&);
template void slice<Eigen::SparseMatrix<double, 0, int>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::SparseMatrix<double, 0, int>>(Eigen::SparseMatrix<double, 0, int> const&, Eigen::DenseBase<Eigen::Matrix<double, -1, 1, 0, -1, 1>> const&, int, Eigen::SparseMatrix<double, 0, int>&);
template void slice<Eigen::SparseMatrix<double, 0, int>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::SparseMatrix<double, 0, int>>(Eigen::SparseMatrix<double, 0, int> const&, Eigen::DenseBase<Eigen::Matrix<int, -1, -1, 0, -1, -1>> const&, int, Eigen::SparseMatrix<double, 0, int>&);
template void slice<Eigen::SparseMatrix<double, 0, int>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::SparseMatrix<double, 0, int>>(Eigen::SparseMatrix<double, 0, int> const&, Eigen::DenseBase<Eigen::Matrix<int, -1, 1, 0, -1, 1>> const&, int, Eigen::SparseMatrix<double, 0, int>&);
#ifdef WIN32
template void slice<unsigned int>(class std::vector<unsigned int, class std::allocator<unsigned int> > const&, class std::vector<unsigned __int64, class std::allocator<unsigned __int64> > const&, class std::vector<unsigned int, class std::allocator<unsigned int> >&);
template void slice<float>(class std::vector<float, class std::allocator<float> > const&, class std::vector<unsigned __int64, class std::allocator<unsigned __int64> > const&, class std::vector<float, class std::allocator<float> >&);
template void slice<__int64>(class std::vector<__int64, class std::allocator<__int64> > const&, class std::vector<unsigned __int64, class std::allocator<unsigned __int64> > const&, class std::vector<__int64, class std::allocator<__int64> >&);
#endif
template void slice<unsigned int>(std::vector<unsigned int, std::allocator<unsigned int> > const&, std::vector<unsigned long, std::allocator<unsigned long> > const&, std::vector<unsigned int, std::allocator<unsigned int> >&);
template void slice<float>(std::vector<float, std::allocator<float> > const&, std::vector<size_t, std::allocator<size_t> > const&, std::vector<float, std::allocator<float> >&);
template void slice<double>(std::vector<double, std::allocator<double> > const&, std::vector<size_t, std::allocator<size_t> > const&, std::vector<double, std::allocator<double> >&);
template void slice<int>(std::vector<int, std::allocator<int> > const&, std::vector<size_t, std::allocator<size_t> > const&, std::vector<int, std::allocator<int> >&);
template void slice<long>(std::vector<long, std::allocator<long> > const&, std::vector<size_t, std::allocator<size_t> > const&, std::vector<long, std::allocator<long> >&);
#include "SortableRow.h"
template void slice<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> > >(std::vector<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> >, std::allocator<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> > > > const&, std::vector<size_t, std::allocator<size_t> > const&, std::vector<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> >, std::allocator<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> > > >&);
template void slice<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> > >(std::vector<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> >, std::allocator<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> > > > const&, std::vector<size_t, std::allocator<size_t> > const&, std::vector<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> >, std::allocator<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> > > >&);
