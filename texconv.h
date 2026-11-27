// texconv.h — declarations for libtexconv.so
#pragma once

#include <cstddef>
#include <cstdint>

struct TexMetadata
{
	size_t			width;
	size_t			height;
	size_t			depth;
	size_t			arraySize;
	size_t			mipLevels;
	uint32_t		miscFlags;
	uint32_t		miscFlags2;
	uint32_t		format;
	uint32_t		dimension;

	size_t ComputeIndex(size_t mip, size_t item, size_t slice) const;
	bool IsCubemap() const;
	bool IsPMAlpha();
	void SetAlphaMode(uint32_t mode);
	bool IsVolumemap();
	uint32_t CalculateSubresource(size_t mip, size_t item) const;
	uint32_t CalculateSubresource(size_t mip, size_t item, size_t plane) const;
};

extern "C" int texconv(int argc, wchar_t* argv[],
						bool verbose = true,
						bool init_com = false,
						bool allow_slow_codec = true,
						wchar_t* err_buf = nullptr,
						int err_buf_size = 0);

extern "C" int texdiag(int argc, wchar_t* argv[], TexMetadata& info,
						bool verbose = true, bool init_com = false,
						wchar_t* err_buf = nullptr, int err_buf_size = 0);

