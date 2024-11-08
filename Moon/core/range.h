#include<limits>
#include<type_traits>
namespace MOON {

	template<typename T>struct RangeIterator {
		explicit RangeIterator(T& in_range) :m_range(in_range) {}
		void operator++() { m_range.advance(); }
		auto operator*() { return m_range.value(); }
		auto operator*()const { return m_range.value(); }
		friend bool operator!=(const RangeIterator& lhs, void* rhs) { return !lhs.m_range.at_end(); }
		friend bool operator!=(void* lhs, const RangeIterator& rhs) { return !rhs.m_range.at_end(); }
		friend bool operator==(const RangeIterator& lhs, void* rhs) { return lhs.m_range.at_end(); }
		friend bool operator==(void* lhs, const RangeIterator& rhs) { return rhs.m_range.at_end(); }
		T& m_range;
	};

	template<typename Devried>struct RangeBase {
		using Iterator = RangeIterator<Devried>;
		Iterator begin() { return Iterator(*static_cast<Devried*>(this)); }
		void* end() const { return nullptr; }
	};
	template<typename Derived> struct RangeActionBase
	{
		template<typename R>  friend auto operator|(R&& range, Derived&& action) { return action(std::forward<R>(range)); }
	};

	template<typename T> struct IndexRange : public RangeBase<IndexRange<T>>
	{
		IndexRange() noexcept : m_index(0), m_end(std::numeric_limits<T>::max()) {}

		explicit IndexRange(T end) noexcept : m_index(0), m_end(end) {}

		explicit IndexRange(T begin, T end) noexcept : m_index(begin), m_end(end) {}

		inline T value() const noexcept { return m_index; }

		bool at_end() const noexcept { return m_index >= m_end; }

		void advance() noexcept { ++m_index; }

	public:
		T m_index;
		T m_end;
	};



}