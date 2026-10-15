#include "byte_stream.hh"
#include <bits/stdc++.h>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return this->w_closed_;
}

void Writer::push( string data )
{
  // Your code here.
  if ( this->is_closed() ) {
    return;
  }

  uint64_t start = 0, n = data.size();
  uint64_t available_capacity = this->available_capacity();

  while ( available_capacity && start < n ) {
    uint64_t sublen = min( n - start, available_capacity );

    string buf = data.substr( start, min( n - start, available_capacity ) );
    this->reader().append_buffer( buf );
    this->bytes_pushed_ += sublen;

    start += sublen;
    available_capacity -= sublen;
  }
  return;
}

void Writer::close()
{
  this->w_closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  uint64_t buf_size = this->reader().get_buffer_size();
  return this->capacity_ - buf_size;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return this->bytes_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  if ( ( this->get_buffer_size() == 0 ) && this->writer().is_closed() )
    return true;
  return false;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return this->bytes_popped_;
}

string_view Reader::peek() const
{
  // Your code here.
  uint64_t buf_size = this->get_buffer_size();
  if ( !buf_size ) {
    // this->set_error();
    return "\0";
  }
  return string_view { buffer.data() + bytes_popped_, this->get_buffer_size() };
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  uint64_t n = this->get_buffer_size();
  if ( n < len ) {
    this->set_error();
    return;
  }

  this->bytes_popped_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return this->get_buffer_size();
}
