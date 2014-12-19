#include <ClientServerFramework/Client/Scraper.hpp>

#include <ClientServerStringReversal/Shared/Data.h>

class DataScraper :
	public ClientServer::Scraper<data>
{
public:
	DataScraper(std::istream& is); 
};