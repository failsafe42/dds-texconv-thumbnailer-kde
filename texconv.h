// texconv.h — declarations for libtexconv.so
#pragma once

extern "C" int texconv(int argc, wchar_t* argv[],
			bool verbose = true,
			bool init_com = false,
			bool allow_slow_codec = true,
			wchar_t* err_buf = nullptr,
			int err_buf_size = 0);
