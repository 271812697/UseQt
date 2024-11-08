#pragma once
#include <array>
#include <utility>
#include <cstring>
#include <cstdio>
#include <cstdint>

template <typename T>
void tfread(FILE* in, const T& t)
{
	[[maybe_unused]] auto n_items = fread((char*)&t, 1, sizeof(t), in);
}

template <typename T>
void tfwrite(FILE* out, const T& t)
{
	[[maybe_unused]] auto n_items = fwrite((char*)&t, 1, sizeof(t), out);
}

inline unsigned int byteswap32(unsigned int val)
{
#if defined(__has_builtin)
#define HAS_BUILTIN(x) __has_builtin(x)
#else
#define HAS_BUILTIN(x) 0
#endif
#if !defined(_MSC_VER) && HAS_BUILTIN(__builtin_bswap32)
	return __builtin_bswap32(val);
#elif defined(_MSC_VER)
	return _byteswap_ulong(val);
#else
	std::array<uint8_t, 4> bytes;
	uint_t ret;
	std::memcpy(bytes.data(), &val, 4);
	std::swap(bytes[0], bytes[3]);
	std::swap(bytes[1], bytes[2]);
	std::memcpy(&ret, bytes.data(), 4);
	return ret;
#endif
#undef HAS_BUILTIN
}
