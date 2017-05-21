#pragma once

#define NOMINMAX
#undef max

template <typename T>
class UntrackedAllocator {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

public:
	template<typename U>
	struct rebind {
		typedef UntrackedAllocator<U> other;
	};

public:
	inline explicit UntrackedAllocator() {}
	inline ~UntrackedAllocator() {}
	inline UntrackedAllocator(UntrackedAllocator const&) {}
	template<typename U>
	inline UntrackedAllocator(UntrackedAllocator<U> const&) {}

	//    address
	inline pointer address(reference r) {
		return &r;
	}

	inline const_pointer address(const_reference r) {
		return &r;
	}

	//    memory allocation
	inline pointer allocate(size_type cnt,
		typename std::allocator<void>::const_pointer = 0) {
		T *ptr = (T*)malloc(cnt * sizeof(T));
		return ptr;
	}

	inline void deallocate(pointer p, size_type cnt) {
		cnt;
		free(p);
	}

	//   size
	inline size_type max_size() const {
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}

	// construction/destruction
	inline void construct(pointer p, const T& t) {
		new(p) T(t);
	}

	inline void destroy(pointer p) {
		p;
		p->~T();
	}

};

// return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator== (const UntrackedAllocator<T1>&, const UntrackedAllocator<T2>&) throw() {
	return true;
}
template <class T1, class T2>
bool operator!= (const UntrackedAllocator<T1>&, const UntrackedAllocator<T2>&) throw() {
	return false;
}