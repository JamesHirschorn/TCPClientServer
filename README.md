TCPClientServer
===============

Copyright (C) 2015 James Hirschorn <James.Hirschorn@quantitative-technologies.com>
----------------------------------------------------------------------------------

Overview
--------

This source package contains two projects. The project in the subfolder 
"ClientServerFramework" is a header only library that provides all of the 
functionality needed in our client/server TCP communication framework. This	
includes network communication, data scraping and queuing, serialization, 
encryption and compression. The client-side scraper is run on a separate 
thread and the messages are queued for sending in the order they are scraped.
The server side uses asynchronous communication so that multiple clients can 
connect at the same time. The client side uses blocking communication calls. 
The framework provides the choice between plain sockets and encrypted SSL 
sockets, and it provides filters for compression using zlib. Moreover, the 
design is modular and would allow new sockets types or new compression 
algorithms to be added with minimal changes to the code.

All that is needed to create an application based on this framework is:
1. Definitions of the client and server data types, i.e. the type of the data
transmitted by the client and server, respectively. These can be structs 
composed of any types so long as they satisfy the Boost.Serialization concept. 
2. Client and server strategies (i.e. as in the strategy pattern). The client
strategy specifies what do to with the scraped data before sending it to the 
server, and what to do with the server response to the sent data. The server 
strategy creates a response from the data received from the client.
3. A concrete implementation of the data scraper class, that reads in objects
of the client data type from an input stream.

The other project is a concrete application in the subdirectory 
"ClientServerCalculator". It is a simple calculator program: The client sends
two double operands and an arithmetic operation (+, -, *, /) and the server
computes the result of the given calculation, and sends back the result 
together with a time stamp indicating when the calculation was done. The 
client also sends a request id that is return by the server for 
identification. There is also a client id field and a "padding" field. The 
padding contains a relatively large amount of data (2000 bytes in the example 
code) that is sent to the server and returned unchanged in the response to the 
client. The purpose of the padding is to demonstrate compression (small 
amounts of data do not compress to a smaller size due to overhead).

Note that the exact same formula could be used to build a more sophisticated
client/server application, using the ClientServerFramework library as is.

Build
-----

The CMake build system was used for this project. This means that the server
and client can run on any pair of operating systems that have a sufficiently
compliant C++11 compiler. The required libraries are Boost, OpenSSL and zlib.
The framework itself only depends on Boost and OpenSSL. For Windows builds,
the OpenSSL from http://www.slproweb.com/products/Win32OpenSSL.html should be
used to make sure it works with CMake. 

The only tricky thing is that boost must be built with zlib support, which is
off by default in Windows builds (on in Linux), and is not well documented by 
Boost. The necessary boost library builds are: iostreams, system, thread, 
date_time and serialization. On my Windows system the boost build was done 
with the command:

``bjam -sZLIB_SOURCE="C:\zlib-1.2.8" --with-iostreams --with-system           \``       
` ` ` ` ` ` ``--with-thread --with-date_time --with-serialization                  \``    
` ` ` ` ` ` ``--debug-configuration stage``

where the zlib library was installed in C:\zlib-1.2.8. Note that there cannot
be any spaces in the zlib library path. On Linux:
	
``./bjam -sNO_BZIP2=1 --with-iostreams --with-system                          \  
	--with-thread --with-date_time --with-serialization                 \  
	--debug-configuration stage``

Note: The project has been built on Visual Studio 2013 and Debian Linux. The 
stable version of Debian was used; however, the complier had to be upgraded
to gcc-4.9, as gcc-4.7 has known bugs that prevented compilation. There may 
be minor changes needed other builds.

Usage
-----

A brief usage explanation can be obtained by running any of the three 
executables with no command line arguments. They are Calculator-TCPClient 
(the client), Calculator-TCPServer (the server) and DataGenerator which 
generates a stream for the client to scrape. Some basic examples:

Calculator-TCPServer.exe 12345

runs the server listening on port 12345. 

DataGenerator.exe 10 | Calculator-TCPClient.exe James localhost 12345

runs a client with client ID "James" connecting to localhost port 12345, 
with 10 requests fed to the client.

Multiple clients can be run simultaneously. 

Design
------

The source code contains descriptions in the headings. The design is highly 
modular. Straightforward design patterns are used to ensure maintainability.
Most significantly, the strategy pattern is used to determine the socket 
protocol, e.g. SSL or plain sockets. The sockets themselves are completely 
encapsulated in IO_base and the derived classes (SSL_IO.hpp and 
Unsecured.hpp). The strategy pattern is also used to specify input and 
output filters. This is currently used for compression (see Filter_base.hpp,
DefaultFilter.hpp and CompressionFilter.hpp). A singleton is used for the 
client message queue.

The code is mostly very simple with short code files. The "heavy-lifting" is
mostly in Connection_base.hpp.
