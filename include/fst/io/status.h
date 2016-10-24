#pragma once

namespace fst {
namespace io {

	enum class error {
		kNone,
		kResourceTemporarilyUnavailable,
		kProtocolFamilyNotSupported,
		kAddressFamilyNotSupported,
		kOperationTimedOut,
		kOperationNotPermitted,
		kNoSuchFileOrDirectory,
		kSenderWasClosed,
		kReaderWasClosed,
		kBrokenPipe,
		kInvalidFileDescriptor,
		kUnknown
	};

	enum class status { good, warning, bad };

} // io.
} // fst.
