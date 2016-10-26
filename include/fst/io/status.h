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

	enum class state { good, warning, bad };

	struct status {
		state state;
		error error;
	};

} // io.
} // fst.
