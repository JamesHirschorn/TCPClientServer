/** The serializable data structure sent by the client. 
 *	
 *	Note: The operation_type is represented by a char 
 *		  in the input data stream.
 */

#ifndef CALCULATOR_TCPCLIENT_CLIENT_DATA_HPP
#define CALCULATOR_TCPCLIENT_CLIENT_DATA_HPP

#include <cstdint>
#include <iostream>
#include <string>

struct client_data
{
	enum operation_enum {NOP = 0, ADD, SUBTRACT, MULTIPLY, DIVIDE};

	class operation_type
	{
		operation_enum operation_;
	public:
		/// default ctor
		operation_type() : operation_(NOP)
		{}
		/// conversion ctor
		operation_type(operation_enum op) : operation_(op)
		{}

		/// implicit conversion to operation_enum
		operator operation_enum() const
		{
			return operation_;
		}

		/// implicit conversion to std::string
		operator std::string() const
		{
			switch (operation_)
			{
			case NOP:
				return "NOP";
			case ADD:
				return "+";
			case SUBTRACT:
				return "-";
			case MULTIPLY:
				return "*";
			case DIVIDE:
				return "/";
			default:
				return "undefined";
			}
		}	
		
		/// to satisfy serializable concept
		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & operation_;
		}
	};

	std::string		client_id;	// client id
	int32_t			id;			// request id
	double			operand1;	// first operand
	operation_type	operation;	// operation (+, -, *, /)
	double			operand2;	// second operand

	/// to satisfy serializable concept
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & client_id;
		ar & id;
		ar & operand1;
		ar & operation;
		ar & operand2;
	}
};

/// overload << for operation_type
std::ostream& operator<<(std::ostream& os, client_data::operation_type operation);

/// overload << for client_data
std::ostream& operator<<(std::ostream& os, client_data data);

#endif	// !CALCULATOR_TCPCLIENT_CLIENT_DATA_HPP