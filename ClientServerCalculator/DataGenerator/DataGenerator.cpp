/** 
 *	The purpose of this program is to generates data to be used as input 
 *	to the Calculator client.
 *
 *	The data is randomly generated, and the generated sequence will be the
 *	for every run with a given seed value. It occassionally makes an
 *	intentional bad data entry.
 *
 *	Usage: DataGenerator <data size> [<seed>] [<sigma>]
 *
 *	seed is the optional random number seed, and sigma determines
 *	the distribution of the operands as N(0,sigma^2), i.e. normally 
 *  distributed.
 */

#include <ClientServerCalculator/TCPClient/client_data.hpp>
#include <ClientServerCalculator/TCPClient/client_data.cpp>	// include external definitions

#include <array>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

double const default_sigma = 1000000;

int main(int argc, char *argv[])
{
	using namespace std;

	size_t data_size;
	auto seed = mt19937_64::default_seed;
	auto sigma = default_sigma;
	
	if (argc <= 1)
	{
		cerr << "Usage: " << argv[0] << " <data size> [<seed>] [<sigma>]" << endl;
		exit(EXIT_FAILURE);
	}
	data_size = atoi(argv[1]);

	if (argc > 2)
	{
		seed = atoi(argv[2]);
	}
	if (argc > 3)
	{
		sigma = atof(argv[3]);
	}

	mt19937_64 random_engine(seed);

	// The first value corresponds to the invalid NOP operation, 
	// and we make it half as likely to occur as the valid operations.
	discrete_distribution<> operation_dist{ 0.5, 1, 1, 1, 1 };

	normal_distribution<> operand_dist(0, sigma);

	// Padding consists of the following string repeated.
	string word = "The padding...";
	array<char, client_data::padding_size> padding;
	for (size_t i = 0; i < client_data::padding_size; ++i)
	{
		padding[i] = word[i % word.size()];
	}

	for (int request_id = 1; request_id <= data_size; ++request_id)
	{
		auto operation = static_cast<client_data::operation_enum>(operation_dist(random_engine));
		auto operand1 = operand_dist(random_engine);
		auto operand2 = operand_dist(random_engine);

		client_data datum = { "", request_id, operand1, operation, operand2, padding };

		cout << datum << endl;
	}
}