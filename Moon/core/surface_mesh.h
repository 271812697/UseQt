#pragma once
#include <cassert>
#include <cstddef>
#include <compare>
#include <filesystem>
#include <iterator>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "core/types.h"
#include "core/properties.h"
#include "core/exceptions.h"

namespace  MOON {

	struct IOFlags;
	class Handle
	{
	public:

		explicit Handle(IndexType idx = MAX_INDEX) : idx_(idx) {}

		IndexType idx() const { return idx_; }

		void reset() { idx_ = MAX_INDEX; }

		bool is_valid() const { return idx_ != MAX_INDEX; }

		bool operator<(const Handle& rhs) const {
			return idx_ < rhs.idx_;
		}
		bool operator>(const Handle& rhs) const {
			return idx_ > rhs.idx_;
		}
		bool operator==(const Handle& rhs) const {
			return idx_ == rhs.idx_;
		}
		bool operator!=(const Handle& rhs) const {
			return idx_ != rhs.idx_;
		}


	private:
		friend class SurfaceMesh;
		IndexType idx_;
	};

	class Vertex : public Handle
	{
		using Handle::Handle;
	};

	class Halfedge : public Handle
	{
		using Handle::Handle;
	};

	class Edge : public Handle
	{
		using Handle::Handle;
	};

	class Face : public Handle
	{
		using Handle::Handle;
	};

	inline std::ostream& operator<<(std::ostream& os, Vertex v)
	{
		return (os << 'v' << v.idx());
	}

	inline std::ostream& operator<<(std::ostream& os, Halfedge h)
	{
		return (os << 'h' << h.idx());
	}

	inline std::ostream& operator<<(std::ostream& os, Edge e)
	{
		return (os << 'e' << e.idx());
	}

	inline std::ostream& operator<<(std::ostream& os, Face f)
	{
		return (os << 'f' << f.idx());
	}

	template <class T>
	class VertexProperty : public Property<T>
	{
	public:
		explicit VertexProperty() = default;
		explicit VertexProperty(Property<T> p) : Property<T>(p) {}

		typename Property<T>::reference operator[](Vertex v)
		{
			return Property<T>::operator[](v.idx());
		}

		typename Property<T>::const_reference operator[](Vertex v) const
		{
			return Property<T>::operator[](v.idx());
		}
	};

	template <class T>
	class HalfedgeProperty : public Property<T>
	{
	public:
		explicit HalfedgeProperty() = default;
		explicit HalfedgeProperty(Property<T> p) : Property<T>(p) {}

		typename Property<T>::reference operator[](Halfedge h)
		{
			return Property<T>::operator[](h.idx());
		}

		typename Property<T>::const_reference operator[](Halfedge h) const
		{
			return Property<T>::operator[](h.idx());
		}
	};

	template <class T>
	class EdgeProperty : public Property<T>
	{
	public:

		explicit EdgeProperty() = default;
		explicit EdgeProperty(Property<T> p) : Property<T>(p) {}

		typename Property<T>::reference operator[](Edge e)
		{
			return Property<T>::operator[](e.idx());
		}

		typename Property<T>::const_reference operator[](Edge e) const
		{
			return Property<T>::operator[](e.idx());
		}
	};

	template <class T>
	class FaceProperty : public Property<T>
	{
	public:

		explicit FaceProperty() = default;
		explicit FaceProperty(Property<T> p) : Property<T>(p) {}

		typename Property<T>::reference operator[](Face f)
		{
			return Property<T>::operator[](f.idx());
		}

		typename Property<T>::const_reference operator[](Face f) const
		{
			return Property<T>::operator[](f.idx());
		}
	};

	class SurfaceMesh
	{
	public:
		class VertexIterator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Vertex;
			using reference = Vertex&;
			using pointer = Vertex*;
			using iterator_category = std::bidirectional_iterator_tag;

			VertexIterator(Vertex v = Vertex(), const SurfaceMesh* m = nullptr)
				: handle_(v), mesh_(m)
			{
				if (mesh_ && mesh_->has_garbage())
					while (mesh_->is_valid(handle_) && mesh_->is_deleted(handle_))
						++handle_.idx_;
			}

			Vertex operator*() const { return handle_; }


			bool operator<(const VertexIterator& rhs) const {
				return handle_ < rhs.handle_;
			}
			bool operator>(const VertexIterator& rhs) const {
				return handle_ > rhs.handle_;
			}
			bool operator==(const VertexIterator& rhs) const {
				return handle_ == rhs.handle_;
			}
			bool operator!=(const VertexIterator& rhs) const {
				return handle_ != rhs.handle_;
			}

			VertexIterator& operator++()
			{
				++handle_.idx_;
				assert(mesh_);
				while (mesh_->has_garbage() && mesh_->is_valid(handle_) &&
					mesh_->is_deleted(handle_))
					++handle_.idx_;
				return *this;
			}

			VertexIterator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			VertexIterator& operator--()
			{
				--handle_.idx_;
				assert(mesh_);
				while (mesh_->has_garbage() && mesh_->is_valid(handle_) &&
					mesh_->is_deleted(handle_))
					--handle_.idx_;
				return *this;
			}

			VertexIterator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

		private:
			Vertex handle_;
			const SurfaceMesh* mesh_;
		};

		class HalfedgeIterator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Halfedge;
			using reference = Halfedge&;
			using pointer = Halfedge*;
			using iterator_category = std::bidirectional_iterator_tag;

			HalfedgeIterator(Halfedge h = Halfedge(),
				const SurfaceMesh* mesh = nullptr)
				: handle_(h), mesh_(mesh)
			{
				if (mesh_ && mesh_->has_garbage())
					while (mesh_->is_valid(handle_) && mesh_->is_deleted(handle_))
						++handle_.idx_;
			}

			Halfedge operator*() const { return handle_; }

			bool operator<(const HalfedgeIterator& rhs) const {
				return handle_ < rhs.handle_;
			}
			bool operator>(const HalfedgeIterator& rhs) const {
				return handle_ > rhs.handle_;
			}
			bool operator==(const HalfedgeIterator& rhs) const {
				return handle_ == rhs.handle_;
			}
			bool operator!=(const HalfedgeIterator& rhs) const {
				return handle_ != rhs.handle_;
			}

			HalfedgeIterator& operator++()
			{
				++handle_.idx_;
				assert(mesh_);
				while (mesh_->has_garbage() && mesh_->is_valid(handle_) &&
					mesh_->is_deleted(handle_))
					++handle_.idx_;
				return *this;
			}

			HalfedgeIterator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			HalfedgeIterator& operator--()
			{
				--handle_.idx_;
				assert(mesh_);
				while (mesh_->has_garbage() && mesh_->is_valid(handle_) &&
					mesh_->is_deleted(handle_))
					--handle_.idx_;
				return *this;
			}
			HalfedgeIterator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

		private:
			Halfedge handle_;
			const SurfaceMesh* mesh_;
		};

		class EdgeIterator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Edge;
			using reference = Edge&;
			using pointer = Edge*;
			using iterator_category = std::bidirectional_iterator_tag;

			EdgeIterator(Edge e = Edge(), const SurfaceMesh* mesh = nullptr)
				: handle_(e), mesh_(mesh)
			{
				if (mesh_ && mesh_->has_garbage())
					while (mesh_->is_valid(handle_) && mesh_->is_deleted(handle_))
						++handle_.idx_;
			}

			Edge operator*() const { return handle_; }

			bool operator<(const EdgeIterator& rhs) const {
				return handle_ < rhs.handle_;
			}
			bool operator>(const EdgeIterator& rhs) const {
				return handle_ > rhs.handle_;
			}
			bool operator==(const EdgeIterator& rhs) const {
				return handle_ == rhs.handle_;
			}
			bool operator!=(const EdgeIterator& rhs) const {
				return handle_ == rhs.handle_;
			}

			EdgeIterator& operator++()
			{
				++handle_.idx_;
				assert(mesh_);
				while (mesh_->has_garbage() && mesh_->is_valid(handle_) &&
					mesh_->is_deleted(handle_))
					++handle_.idx_;
				return *this;
			}

			EdgeIterator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			EdgeIterator& operator--()
			{
				--handle_.idx_;
				assert(mesh_);
				while (mesh_->has_garbage() && mesh_->is_valid(handle_) &&
					mesh_->is_deleted(handle_))
					--handle_.idx_;
				return *this;
			}

			EdgeIterator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

		private:
			Edge handle_;
			const SurfaceMesh* mesh_;
		};

		class FaceIterator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Face;
			using reference = Face&;
			using pointer = Face*;
			using iterator_category = std::bidirectional_iterator_tag;

			FaceIterator(Face f = Face(), const SurfaceMesh* m = nullptr)
				: handle_(f), mesh_(m)
			{
				if (mesh_ && mesh_->has_garbage())
					while (mesh_->is_valid(handle_) && mesh_->is_deleted(handle_))
						++handle_.idx_;
			}

			Face operator*() const { return handle_; }

			bool operator<(const FaceIterator& rhs) const {
				return handle_ < rhs.handle_;
			}
			bool operator>(const FaceIterator& rhs) const {
				return handle_ > rhs.handle_;
			}
			bool operator==(const FaceIterator& rhs) const {
				return handle_ == rhs.handle_;
			}
			bool operator!=(const FaceIterator& rhs) const {
				return handle_ != rhs.handle_;
			}
			FaceIterator& operator++()
			{
				++handle_.idx_;
				assert(mesh_);
				while (mesh_->has_garbage() && mesh_->is_valid(handle_) &&
					mesh_->is_deleted(handle_))
					++handle_.idx_;
				return *this;
			}

			FaceIterator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			FaceIterator& operator--()
			{
				--handle_.idx_;
				assert(mesh_);
				while (mesh_->has_garbage() && mesh_->is_valid(handle_) &&
					mesh_->is_deleted(handle_))
					--handle_.idx_;
				return *this;
			}

			FaceIterator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

		private:
			Face handle_;
			const SurfaceMesh* mesh_;
		};

		class VertexContainer
		{
		public:
			VertexContainer(VertexIterator begin, VertexIterator end)
				: begin_(begin), end_(end)
			{
			}
			VertexIterator begin() const { return begin_; }
			VertexIterator end() const { return end_; }

		private:
			VertexIterator begin_;
			VertexIterator end_;
		};

		class HalfedgeContainer
		{
		public:
			HalfedgeContainer(HalfedgeIterator begin, HalfedgeIterator end)
				: begin_(begin), end_(end)
			{
			}
			HalfedgeIterator begin() const { return begin_; }
			HalfedgeIterator end() const { return end_; }

		private:
			HalfedgeIterator begin_;
			HalfedgeIterator end_;
		};

		class EdgeContainer
		{
		public:
			EdgeContainer(EdgeIterator begin, EdgeIterator end)
				: begin_(begin), end_(end)
			{
			}
			EdgeIterator begin() const { return begin_; }
			EdgeIterator end() const { return end_; }

		private:
			EdgeIterator begin_;
			EdgeIterator end_;
		};

		class FaceContainer
		{
		public:
			FaceContainer(FaceIterator begin, FaceIterator end)
				: begin_(begin), end_(end)
			{
			}
			FaceIterator begin() const { return begin_; }
			FaceIterator end() const { return end_; }

		private:
			FaceIterator begin_;
			FaceIterator end_;
		};


		class VertexAroundVertexCirculator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Vertex;
			using reference = Vertex&;
			using pointer = Vertex*;
			using iterator_category = std::bidirectional_iterator_tag;

			VertexAroundVertexCirculator(const SurfaceMesh* mesh = nullptr,
				Vertex v = Vertex())
				: mesh_(mesh)
			{
				if (mesh_)
					halfedge_ = mesh_->halfedge(v);
			}

			bool operator==(const VertexAroundVertexCirculator& rhs) const
			{
				assert(mesh_);
				assert(mesh_ == rhs.mesh_);
				return (is_active_ && (halfedge_ == rhs.halfedge_));
			}

			bool operator!=(const VertexAroundVertexCirculator& rhs) const
			{
				return !operator==(rhs);
			}

			VertexAroundVertexCirculator& operator++()
			{
				assert(mesh_);
				halfedge_ = mesh_->ccw_rotated_halfedge(halfedge_);
				is_active_ = true;
				return *this;
			}

			VertexAroundVertexCirculator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			VertexAroundVertexCirculator& operator--()
			{
				assert(mesh_);
				halfedge_ = mesh_->cw_rotated_halfedge(halfedge_);
				return *this;
			}

			VertexAroundVertexCirculator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

			Vertex operator*() const
			{
				assert(mesh_);
				return mesh_->to_vertex(halfedge_);
			}

			operator bool() const { return halfedge_.is_valid(); }

			Halfedge halfedge() const { return halfedge_; }

			VertexAroundVertexCirculator& begin()
			{
				is_active_ = !halfedge_.is_valid();
				return *this;
			}

			VertexAroundVertexCirculator& end()
			{
				is_active_ = true;
				return *this;
			}

		private:
			const SurfaceMesh* mesh_;
			Halfedge halfedge_;
			bool is_active_{ true };
		};

		class HalfedgeAroundVertexCirculator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Halfedge;
			using reference = Halfedge&;
			using pointer = Halfedge*;
			using iterator_category = std::bidirectional_iterator_tag;

			HalfedgeAroundVertexCirculator(const SurfaceMesh* mesh = nullptr,
				Vertex v = Vertex())
				: mesh_(mesh)
			{
				if (mesh_)
					halfedge_ = mesh_->halfedge(v);
			}

			bool operator==(const HalfedgeAroundVertexCirculator& rhs) const
			{
				assert(mesh_);
				assert(mesh_ == rhs.mesh_);
				return (is_active_ && (halfedge_ == rhs.halfedge_));
			}

			bool operator!=(const HalfedgeAroundVertexCirculator& rhs) const
			{
				return !operator==(rhs);
			}

			HalfedgeAroundVertexCirculator& operator++()
			{
				assert(mesh_);
				halfedge_ = mesh_->ccw_rotated_halfedge(halfedge_);
				is_active_ = true;
				return *this;
			}

			HalfedgeAroundVertexCirculator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			HalfedgeAroundVertexCirculator& operator--()
			{
				assert(mesh_);
				halfedge_ = mesh_->cw_rotated_halfedge(halfedge_);
				return *this;
			}

			HalfedgeAroundVertexCirculator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

			Halfedge operator*() const { return halfedge_; }

			operator bool() const { return halfedge_.is_valid(); }

			HalfedgeAroundVertexCirculator& begin()
			{
				is_active_ = !halfedge_.is_valid();
				return *this;
			}

			HalfedgeAroundVertexCirculator& end()
			{
				is_active_ = true;
				return *this;
			}

		private:
			const SurfaceMesh* mesh_;
			Halfedge halfedge_;
			bool is_active_{ true };
		};

		class EdgeAroundVertexCirculator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Edge;
			using reference = Edge&;
			using pointer = Edge*;
			using iterator_category = std::bidirectional_iterator_tag;

			EdgeAroundVertexCirculator(const SurfaceMesh* mesh = nullptr,
				Vertex v = Vertex())
				: mesh_(mesh)
			{
				if (mesh_)
					halfedge_ = mesh_->halfedge(v);
			}

			bool operator==(const EdgeAroundVertexCirculator& rhs) const
			{
				assert(mesh_);
				assert(mesh_ == rhs.mesh_);
				return (is_active_ && (halfedge_ == rhs.halfedge_));
			}

			bool operator!=(const EdgeAroundVertexCirculator& rhs) const
			{
				return !operator==(rhs);
			}

			EdgeAroundVertexCirculator& operator++()
			{
				assert(mesh_);
				halfedge_ = mesh_->ccw_rotated_halfedge(halfedge_);
				is_active_ = true;
				return *this;
			}

			EdgeAroundVertexCirculator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			EdgeAroundVertexCirculator& operator--()
			{
				assert(mesh_);
				halfedge_ = mesh_->cw_rotated_halfedge(halfedge_);
				return *this;
			}

			EdgeAroundVertexCirculator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

			Edge operator*() const { return mesh_->edge(halfedge_); }

			operator bool() const { return halfedge_.is_valid(); }

			EdgeAroundVertexCirculator& begin()
			{
				is_active_ = !halfedge_.is_valid();
				return *this;
			}

			EdgeAroundVertexCirculator& end()
			{
				is_active_ = true;
				return *this;
			}

		private:
			const SurfaceMesh* mesh_;
			Halfedge halfedge_;
			bool is_active_{ true };
		};

		class FaceAroundVertexCirculator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Face;
			using reference = Face&;
			using pointer = Face*;
			using iterator_category = std::bidirectional_iterator_tag;

			FaceAroundVertexCirculator(const SurfaceMesh* m = nullptr,
				Vertex v = Vertex())
				: mesh_(m)
			{
				if (mesh_)
				{
					halfedge_ = mesh_->halfedge(v);
					if (halfedge_.is_valid() && mesh_->is_boundary(halfedge_))
						operator++();
				}
			}

			bool operator==(const FaceAroundVertexCirculator& rhs) const
			{
				assert(mesh_);
				assert(mesh_ == rhs.mesh_);
				return (is_active_ && (halfedge_ == rhs.halfedge_));
			}

			bool operator!=(const FaceAroundVertexCirculator& rhs) const
			{
				return !operator==(rhs);
			}

			FaceAroundVertexCirculator& operator++()
			{
				assert(mesh_ && halfedge_.is_valid());
				do
				{
					halfedge_ = mesh_->ccw_rotated_halfedge(halfedge_);
				} while (mesh_->is_boundary(halfedge_));
				is_active_ = true;
				return *this;
			}

			FaceAroundVertexCirculator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			FaceAroundVertexCirculator& operator--()
			{
				assert(mesh_ && halfedge_.is_valid());
				do
					halfedge_ = mesh_->cw_rotated_halfedge(halfedge_);
				while (mesh_->is_boundary(halfedge_));
				return *this;
			}

			FaceAroundVertexCirculator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

			Face operator*() const
			{
				assert(mesh_ && halfedge_.is_valid());
				return mesh_->face(halfedge_);
			}

			operator bool() const { return halfedge_.is_valid(); }

			FaceAroundVertexCirculator& begin()
			{
				is_active_ = !halfedge_.is_valid();
				return *this;
			}

			FaceAroundVertexCirculator& end()
			{
				is_active_ = true;
				return *this;
			}

		private:
			const SurfaceMesh* mesh_;
			Halfedge halfedge_;
			bool is_active_{ true };
		};

		class VertexAroundFaceCirculator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Vertex;
			using reference = Vertex&;
			using pointer = Vertex*;
			using iterator_category = std::bidirectional_iterator_tag;

			VertexAroundFaceCirculator(const SurfaceMesh* m = nullptr,
				Face f = Face())
				: mesh_(m)
			{
				if (mesh_)
					halfedge_ = mesh_->halfedge(f);
			}

			bool operator==(const VertexAroundFaceCirculator& rhs) const
			{
				assert(mesh_);
				assert(mesh_ == rhs.mesh_);
				return (is_active_ && (halfedge_ == rhs.halfedge_));
			}

			bool operator!=(const VertexAroundFaceCirculator& rhs) const
			{
				return !operator==(rhs);
			}

			VertexAroundFaceCirculator& operator++()
			{
				assert(mesh_ && halfedge_.is_valid());
				halfedge_ = mesh_->next_halfedge(halfedge_);
				is_active_ = true;
				return *this;
			}

			VertexAroundFaceCirculator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			VertexAroundFaceCirculator& operator--()
			{
				assert(mesh_ && halfedge_.is_valid());
				halfedge_ = mesh_->prev_halfedge(halfedge_);
				return *this;
			}

			VertexAroundFaceCirculator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

			Vertex operator*() const
			{
				assert(mesh_ && halfedge_.is_valid());
				return mesh_->to_vertex(halfedge_);
			}

			VertexAroundFaceCirculator& begin()
			{
				is_active_ = false;
				return *this;
			}

			VertexAroundFaceCirculator& end()
			{
				is_active_ = true;
				return *this;
			}

		private:
			const SurfaceMesh* mesh_;
			Halfedge halfedge_;
			bool is_active_{ true };
		};

		class HalfedgeAroundFaceCirculator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = Halfedge;
			using reference = Halfedge&;
			using pointer = Halfedge*;
			using iterator_category = std::bidirectional_iterator_tag;

			HalfedgeAroundFaceCirculator(const SurfaceMesh* m = nullptr,
				Face f = Face())
				: mesh_(m)
			{
				if (mesh_)
					halfedge_ = mesh_->halfedge(f);
			}

			bool operator==(const HalfedgeAroundFaceCirculator& rhs) const
			{
				assert(mesh_);
				assert(mesh_ == rhs.mesh_);
				return (is_active_ && (halfedge_ == rhs.halfedge_));
			}

			bool operator!=(const HalfedgeAroundFaceCirculator& rhs) const
			{
				return !operator==(rhs);
			}

			HalfedgeAroundFaceCirculator& operator++()
			{
				assert(mesh_ && halfedge_.is_valid());
				halfedge_ = mesh_->next_halfedge(halfedge_);
				is_active_ = true;
				return *this;
			}

			HalfedgeAroundFaceCirculator operator++(int)
			{
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			HalfedgeAroundFaceCirculator& operator--()
			{
				assert(mesh_ && halfedge_.is_valid());
				halfedge_ = mesh_->prev_halfedge(halfedge_);
				return *this;
			}

			HalfedgeAroundFaceCirculator operator--(int)
			{
				auto tmp = *this;
				--(*this);
				return tmp;
			}

			Halfedge operator*() const { return halfedge_; }

			HalfedgeAroundFaceCirculator& begin()
			{
				is_active_ = false;
				return *this;
			}

			HalfedgeAroundFaceCirculator& end()
			{
				is_active_ = true;
				return *this;
			}

		private:
			const SurfaceMesh* mesh_;
			Halfedge halfedge_;
			bool is_active_{ true };
		};

		SurfaceMesh();

		virtual ~SurfaceMesh();

		SurfaceMesh(const SurfaceMesh& rhs) { operator=(rhs); }

		SurfaceMesh& operator=(const SurfaceMesh& rhs);

		SurfaceMesh& assign(const SurfaceMesh& rhs);

		Vertex add_vertex(const Point& p);

		Face add_face(const std::vector<Vertex>& vertices);

		Face add_triangle(Vertex v0, Vertex v1, Vertex v2);

		Face add_quad(Vertex v0, Vertex v1, Vertex v2, Vertex v3);

		size_t vertices_size() const { return vprops_.size(); }

		size_t halfedges_size() const { return hprops_.size(); }

		size_t edges_size() const { return eprops_.size(); }

		size_t faces_size() const { return fprops_.size(); }

		size_t n_vertices() const { return vertices_size() - deleted_vertices_; }

		size_t n_halfedges() const { return halfedges_size() - 2 * deleted_edges_; }

		size_t n_edges() const { return edges_size() - deleted_edges_; }

		size_t n_faces() const { return faces_size() - deleted_faces_; }

		bool is_empty() const { return n_vertices() == 0; }

		virtual void clear();

		void free_memory();

		void reserve(size_t nvertices, size_t nedges, size_t nfaces);

		void garbage_collection();

		bool is_deleted(Vertex v) const { return vdeleted_[v]; }

		bool is_deleted(Halfedge h) const { return edeleted_[edge(h)]; }

		bool is_deleted(Edge e) const { return edeleted_[e]; }

		bool is_deleted(Face f) const { return fdeleted_[f]; }

		bool is_valid(Vertex v) const { return v.idx() < vertices_size(); }

		bool is_valid(Halfedge h) const { return h.idx() < halfedges_size(); }

		bool is_valid(Edge e) const { return e.idx() < edges_size(); }

		bool is_valid(Face f) const { return f.idx() < faces_size(); }

		Halfedge halfedge(Vertex v) const { return vconn_[v].halfedge_; }

		void set_halfedge(Vertex v, Halfedge h) { vconn_[v].halfedge_ = h; }

		bool is_boundary(Vertex v) const
		{
			Halfedge h(halfedge(v));
			return (!(h.is_valid() && face(h).is_valid()));
		}

		bool is_isolated(Vertex v) const { return !halfedge(v).is_valid(); }

		bool is_manifold(Vertex v) const
		{
			int n(0);
			auto hit = halfedges(v);
			auto hend = hit;
			if (hit)
				do
				{
					if (is_boundary(*hit))
						++n;
				} while (++hit != hend);
				return n < 2;
		}

		inline Vertex to_vertex(Halfedge h) const { return hconn_[h].vertex_; }

		inline Vertex from_vertex(Halfedge h) const
		{
			return to_vertex(opposite_halfedge(h));
		}

		inline void set_vertex(Halfedge h, Vertex v) { hconn_[h].vertex_ = v; }

		Face face(Halfedge h) const { return hconn_[h].face_; }

		void set_face(Halfedge h, Face f) { hconn_[h].face_ = f; }

		inline Halfedge next_halfedge(Halfedge h) const
		{
			return hconn_[h].next_halfedge_;
		}

		inline void set_next_halfedge(Halfedge h, Halfedge nh)
		{
			hconn_[h].next_halfedge_ = nh;
			hconn_[nh].prev_halfedge_ = h;
		}

		inline void set_prev_halfedge(Halfedge h, Halfedge ph)
		{
			hconn_[h].prev_halfedge_ = ph;
			hconn_[ph].next_halfedge_ = h;
		}

		inline Halfedge prev_halfedge(Halfedge h) const
		{
			return hconn_[h].prev_halfedge_;
		}

		inline Halfedge opposite_halfedge(Halfedge h) const
		{
			return Halfedge((h.idx() & 1) ? h.idx() - 1 : h.idx() + 1);
		}

		inline Halfedge ccw_rotated_halfedge(Halfedge h) const
		{
			return opposite_halfedge(prev_halfedge(h));
		}

		inline Halfedge cw_rotated_halfedge(Halfedge h) const
		{
			return next_halfedge(opposite_halfedge(h));
		}

		inline Edge edge(Halfedge h) const { return Edge(h.idx() >> 1); }

		inline bool is_boundary(Halfedge h) const { return !face(h).is_valid(); }

		inline Halfedge halfedge(Edge e, unsigned int i) const
		{
			assert(i <= 1);
			return Halfedge((e.idx() << 1) + i);
		}

		inline Vertex vertex(Edge e, unsigned int i) const
		{
			assert(i <= 1);
			return to_vertex(halfedge(e, i));
		}

		Face face(Edge e, unsigned int i) const
		{
			assert(i <= 1);
			return face(halfedge(e, i));
		}

		bool is_boundary(Edge e) const
		{
			return (is_boundary(halfedge(e, 0)) || is_boundary(halfedge(e, 1)));
		}

		Halfedge halfedge(Face f) const { return fconn_[f].halfedge_; }

		void set_halfedge(Face f, Halfedge h) { fconn_[f].halfedge_ = h; }

		bool is_boundary(Face f) const
		{
			Halfedge h = halfedge(f);
			Halfedge hh = h;
			do
			{
				if (is_boundary(opposite_halfedge(h)))
					return true;
				h = next_halfedge(h);
			} while (h != hh);
			return false;
		}

		template <class T>
		VertexProperty<T> add_vertex_property(const std::string& name,
			const T t = T())
		{
			return VertexProperty<T>(vprops_.add<T>(name, t));
		}

		template <class T>
		VertexProperty<T> get_vertex_property(const std::string& name) const
		{
			return VertexProperty<T>(vprops_.get<T>(name));
		}

		template <class T>
		VertexProperty<T> vertex_property(const std::string& name, const T t = T())
		{
			return VertexProperty<T>(vprops_.get_or_add<T>(name, t));
		}

		template <class T>
		void remove_vertex_property(VertexProperty<T>& p)
		{
			vprops_.remove(p);
		}

		bool has_vertex_property(const std::string& name) const
		{
			return vprops_.exists(name);
		}

		template <class T>
		HalfedgeProperty<T> add_halfedge_property(const std::string& name,
			const T t = T())
		{
			return HalfedgeProperty<T>(hprops_.add<T>(name, t));
		}

		template <class T>
		EdgeProperty<T> add_edge_property(const std::string& name, const T t = T())
		{
			return EdgeProperty<T>(eprops_.add<T>(name, t));
		}

		template <class T>
		HalfedgeProperty<T> get_halfedge_property(const std::string& name) const
		{
			return HalfedgeProperty<T>(hprops_.get<T>(name));
		}

		template <class T>
		EdgeProperty<T> get_edge_property(const std::string& name) const
		{
			return EdgeProperty<T>(eprops_.get<T>(name));
		}

		template <class T>
		HalfedgeProperty<T> halfedge_property(const std::string& name,
			const T t = T())
		{
			return HalfedgeProperty<T>(hprops_.get_or_add<T>(name, t));
		}

		template <class T>
		EdgeProperty<T> edge_property(const std::string& name, const T t = T())
		{
			return EdgeProperty<T>(eprops_.get_or_add<T>(name, t));
		}

		template <class T>
		void remove_halfedge_property(HalfedgeProperty<T>& p)
		{
			hprops_.remove(p);
		}

		bool has_halfedge_property(const std::string& name) const
		{
			return hprops_.exists(name);
		}

		template <class T>
		void remove_edge_property(EdgeProperty<T>& p)
		{
			eprops_.remove(p);
		}

		bool has_edge_property(const std::string& name) const
		{
			return eprops_.exists(name);
		}

		std::vector<std::string> vertex_properties() const
		{
			return vprops_.properties();
		}

		std::vector<std::string> halfedge_properties() const
		{
			return hprops_.properties();
		}

		std::vector<std::string> edge_properties() const
		{
			return eprops_.properties();
		}

		template <class T>
		FaceProperty<T> add_face_property(const std::string& name, const T t = T())
		{
			return FaceProperty<T>(fprops_.add<T>(name, t));
		}

		template <class T>
		FaceProperty<T> get_face_property(const std::string& name) const
		{
			return FaceProperty<T>(fprops_.get<T>(name));
		}

		template <class T>
		FaceProperty<T> face_property(const std::string& name, const T t = T())
		{
			return FaceProperty<T>(fprops_.get_or_add<T>(name, t));
		}

		template <class T>
		void remove_face_property(FaceProperty<T>& p)
		{
			fprops_.remove(p);
		}

		bool has_face_property(const std::string& name) const
		{
			return fprops_.exists(name);
		}

		std::vector<std::string> face_properties() const
		{
			return fprops_.properties();
		}

		VertexIterator vertices_begin() const
		{
			return VertexIterator(Vertex(0), this);
		}

		VertexIterator vertices_end() const
		{
			return VertexIterator(Vertex(static_cast<IndexType>(vertices_size())),
				this);
		}

		VertexContainer vertices() const
		{
			return VertexContainer(vertices_begin(), vertices_end());
		}

		HalfedgeIterator halfedges_begin() const
		{
			return HalfedgeIterator(Halfedge(0), this);
		}

		HalfedgeIterator halfedges_end() const
		{
			return HalfedgeIterator(
				Halfedge(static_cast<IndexType>(halfedges_size())), this);
		}

		HalfedgeContainer halfedges() const
		{
			return HalfedgeContainer(halfedges_begin(), halfedges_end());
		}

		EdgeIterator edges_begin() const { return EdgeIterator(Edge(0), this); }

		EdgeIterator edges_end() const
		{
			return EdgeIterator(Edge(static_cast<IndexType>(edges_size())), this);
		}

		EdgeContainer edges() const
		{
			return EdgeContainer(edges_begin(), edges_end());
		}

		VertexAroundVertexCirculator vertices(Vertex v) const
		{
			return VertexAroundVertexCirculator(this, v);
		}

		EdgeAroundVertexCirculator edges(Vertex v) const
		{
			return EdgeAroundVertexCirculator(this, v);
		}

		HalfedgeAroundVertexCirculator halfedges(Vertex v) const
		{
			return HalfedgeAroundVertexCirculator(this, v);
		}

		FaceIterator faces_begin() const { return FaceIterator(Face(0), this); }

		FaceIterator faces_end() const
		{
			return FaceIterator(Face(static_cast<IndexType>(faces_size())), this);
		}

		FaceContainer faces() const
		{
			return FaceContainer(faces_begin(), faces_end());
		}

		FaceAroundVertexCirculator faces(Vertex v) const
		{
			return FaceAroundVertexCirculator(this, v);
		}

		VertexAroundFaceCirculator vertices(Face f) const
		{
			return VertexAroundFaceCirculator(this, f);
		}

		HalfedgeAroundFaceCirculator halfedges(Face f) const
		{
			return HalfedgeAroundFaceCirculator(this, f);
		}

		Halfedge insert_vertex(Edge e, const Point& p)
		{
			return insert_vertex(halfedge(e, 0), add_vertex(p));
		}

		Halfedge insert_vertex(Edge e, Vertex v)
		{
			return insert_vertex(halfedge(e, 0), v);
		}

		Halfedge insert_vertex(Halfedge h0, Vertex v);

		Halfedge find_halfedge(Vertex start, Vertex end) const;

		Edge find_edge(Vertex a, Vertex b) const;

		bool is_triangle_mesh() const;

		bool is_quad_mesh() const;

		bool is_collapse_ok(Halfedge v0v1) const;

		void collapse(Halfedge h);

		bool is_removal_ok(Edge e) const;

		bool remove_edge(Edge e);

		Vertex split(Face f, const Point& p)
		{
			Vertex v = add_vertex(p);
			split(f, v);
			return v;
		}

		void split(Face f, Vertex v);

		Halfedge split(Edge e, const Point& p) { return split(e, add_vertex(p)); }

		Halfedge split(Edge e, Vertex v);

		Halfedge insert_edge(Halfedge h0, Halfedge h1);

		bool is_flip_ok(Edge e) const;

		void flip(Edge e);

		size_t valence(Vertex v) const;

		size_t valence(Face f) const;

		void delete_vertex(Vertex v);

		void delete_edge(Edge e);

		void delete_face(Face f);

		const Point& position(Vertex v) const { return vpoint_[v]; }

		Point& position(Vertex v) { return vpoint_[v]; }

		std::vector<Point>& positions() { return vpoint_.vector(); }

		Vertex new_vertex()
		{
			if (vertices_size() == MAX_INDEX - 1)
			{
				auto what =
					"SurfaceMesh: cannot allocate vertex, max. index reached";
				throw AllocationException(what);
			}
			vprops_.push_back();
			return Vertex(static_cast<IndexType>(vertices_size()) - 1);
		}

		Halfedge new_edge()
		{
			if (halfedges_size() == MAX_INDEX - 1)
			{
				auto what = "SurfaceMesh: cannot allocate edge, max. index reached";
				throw AllocationException(what);
			}

			eprops_.push_back();
			hprops_.push_back();
			hprops_.push_back();

			Halfedge h0(static_cast<IndexType>(halfedges_size()) - 2);
			Halfedge h1(static_cast<IndexType>(halfedges_size()) - 1);

			return h0;
		}

		Halfedge new_edge(Vertex start, Vertex end)
		{
			assert(start != end);

			if (halfedges_size() == MAX_INDEX - 1)
			{
				auto what = "SurfaceMesh: cannot allocate edge, max. index reached";
				throw AllocationException(what);
			}

			eprops_.push_back();
			hprops_.push_back();
			hprops_.push_back();

			Halfedge h0(static_cast<IndexType>(halfedges_size()) - 2);
			Halfedge h1(static_cast<IndexType>(halfedges_size()) - 1);

			set_vertex(h0, end);
			set_vertex(h1, start);

			return h0;
		}

		Face new_face()
		{
			if (faces_size() == MAX_INDEX - 1)
			{
				auto what = "SurfaceMesh: cannot allocate face, max. index reached";
				throw AllocationException(what);
			}

			fprops_.push_back();
			return Face(static_cast<IndexType>(faces_size()) - 1);
		}

	private:
		struct VertexConnectivity
		{
			Halfedge halfedge_;
		};

		struct HalfedgeConnectivity
		{
			Face face_;
			Vertex vertex_;
			Halfedge next_halfedge_;
			Halfedge prev_halfedge_;
		};

		struct FaceConnectivity
		{
			Halfedge halfedge_;
		};

		void adjust_outgoing_halfedge(Vertex v);

		void remove_edge_helper(Halfedge h);

		void remove_loop_helper(Halfedge h);

		inline bool has_garbage() const { return has_garbage_; }

		friend void read_pmp(SurfaceMesh&, const std::filesystem::path&);
		friend void write_pmp(const SurfaceMesh&, const std::filesystem::path&,
			const IOFlags&);

		PropertyContainer vprops_;
		PropertyContainer hprops_;
		PropertyContainer eprops_;
		PropertyContainer fprops_;

		VertexProperty<Point> vpoint_;

		VertexProperty<VertexConnectivity> vconn_;
		HalfedgeProperty<HalfedgeConnectivity> hconn_;
		FaceProperty<FaceConnectivity> fconn_;

		VertexProperty<bool> vdeleted_;
		EdgeProperty<bool> edeleted_;
		FaceProperty<bool> fdeleted_;

		IndexType deleted_vertices_{ 0 };
		IndexType deleted_edges_{ 0 };
		IndexType deleted_faces_{ 0 };

		bool has_garbage_{ false };

		using NextCacheEntry = std::pair<Halfedge, Halfedge>;
		using NextCache = std::vector<NextCacheEntry>;
		std::vector<Vertex> add_face_vertices_;
		std::vector<Halfedge> add_face_halfedges_;
		std::vector<bool> add_face_is_new_;
		std::vector<bool> add_face_needs_adjust_;
		NextCache add_face_next_cache_;
	};

}
