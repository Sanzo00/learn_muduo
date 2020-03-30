#include "muduo/base/CurrentThread.h"

#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

namespace muduo
{

namespace CurrentThread
{
__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 0;
__thread const char* t_cachedName = "unknow";
static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

string statckTrace(bool demangle)
{
	string stack;
	const int max_frames = 200;
	void* frame[max_frames];
	// 获取当前堆栈地址
	// frame 指针数组(传出), 每个地址指针: 函数名, 地址偏移, 返回地址
	// max_frames 缓冲区的个数, 即设置的调用深度
	// nptrs 实际返回的调用深度
	int nptrs = ::backtrace(frame, max_frames);
	
	// 将地址转换成符号字符串数组
	// frame 地址数组
	// nptrs 有效地址个数
	char** strings = ::backtrace_symbols(frame, nptrs);
	if (strings)
	{
		size_t len = 256;
		char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
		for (int i = 1; i < nptrs; ++i) { // skipping the 0-the, which is this function
			if (demangle) {
				char* left_par = nullptr;
				char* plus = nullptr;
				for (char* p = strings[i]; *p; ++p) {
					if (*p == '(') left_par = p;
					else if (*p == '+') plus = p;
				}

				if (left_par && plus) {
					*plus = '\0';
					int status = 0;
					char* ret = abi::__cxa_demangle(left_par+1, demangled, &len, &status);
					*plus = '+';
					if (status == 0) {
						demangled = ret; // ret coule be realloc()
						stack.append(strings[i], left_par+1);
						stack.append(demangled);
						stack.append(plus);
						stack.push_back('\n');
						continue;
					}
				}
			}
			// Fallback to mangled names
			stack.append(strings[i]);
			stack.push_back('\n');
		}
		free(demangled);
		free(strings);
	}
	return stack;
}
} // namespace CurrentThread
} // namespace muduo
