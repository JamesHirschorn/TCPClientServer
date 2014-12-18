#include <ClientServerFramework/Client/TCPConnector.hpp>

using namespace ClientServer;

int main(int argc, char* argv[])
{
	TCPConnector<int> conn("localhost", "12345");
}