#include "muduo/net/Buffer.h"
#include "muduo/net/SocketsOps.h"
#include <errno.h>
#include <sys/uio.h>

using namespace muduo;
using namespace muduo::net;

const char Buffer::kCRLF[] = "\r\n";
const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
//  struct iovec {
//	    ptr_t iov_base; /* Starting address */
//		size_t iov_len; /* Length in bytes */
//	};
 	char extrabuf[65536];
	struct iovec vec[2];
	const size_t writable = writableBytes();
	vec[0].iov_base = begin() + writerIndex_;
	vec[0].iov_len = writeable;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof extrabuf;
	
	const int iovcnt = (wriable < sizeof extrabuf) ? 2 : 1;
	const ssize_t n = sockets::readv(fd, vec, iovcnt);
	if (n < 0) {
		*savedErrno = errno;
	}else if (implicit_cast<size_t>(n) <= writable) {
		writerIndex += n;
	}else {
		writerIndex_ = buffer_size();
		append(extrabuf, n - writable);
	}
	return n;
}
