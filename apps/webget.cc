#include "socket.hh"

#include <cstdlib>
#include <format>
#include <iostream>
#include <span>
#include <string>

using namespace std;

void get_URL( const string& host, const string& path )
{
  auto tcp = make_unique<TCPSocket>();
  auto destination_addr = make_unique<Address>( host, "http" );

  tcp->connect( *destination_addr );

  constexpr string_view send_payload_fmt = "GET {} HTTP/1.1\r\nHost: {}\r\nConnection: close\r\n\r\n";
  string send_payload_result = format( send_payload_fmt, path, host );
  string_view send_payload = send_payload_result;

  tcp->write( send_payload );

  while ( 1 ) {
    string buffer;
    tcp->read( buffer );
    cout << buffer;

    if ( tcp->eof() )
      break;
  }
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
