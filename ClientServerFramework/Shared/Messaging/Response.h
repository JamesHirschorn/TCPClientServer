/* Base class for server responses. */

namespace ClientServer {

	enum TransmitionStatus {OK, ERROR};

	struct Response
	{
		TransmitionStatus transmition_status;
	};

}	// namespace TCPClientServer