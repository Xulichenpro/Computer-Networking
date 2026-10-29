#include "reassembler.hh"
#include <bits/stdc++.h>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.

  this->is_last_substring_ |= is_last_substring;
  uint64_t sublen = data.size(), next_buffer_index = first_index, last_index = sublen + next_buffer_index;
  uint64_t capacity = this->writer().available_capacity() - this->pending_bytes_;
  
  while (sublen > 0 && capacity > 0){
    auto it = this->buffer_.upper_bound(next_buffer_index);
    uint64_t next_key = -0;
    uint64_t next_len = 0;
    bool is_next = false;
    if (it != this->buffer_.end()){
      next_key = it->first;
      next_len = it->second.first;
      is_next = true;
    }

    if (it != this->buffer_.begin()){
      --it;
      uint64_t data_len = (it->second).first;
      if (it->first + data_len > next_buffer_index){
        sublen -= (it->first + data_len - next_buffer_index);
        next_buffer_index = it->first + data_len;
      }
    }
    if (sublen <= 0){
      break;
    }

    if (is_next && next_buffer_index + sublen > next_key){
      this->buffer_.insert({next_buffer_index, 
                          {next_key - next_buffer_index, 
                          data.substr(next_buffer_index - first_index, min(next_key - next_buffer_index,capacity))}});
      sublen -= (next_key - next_buffer_index + next_len);
      next_buffer_index = next_key + next_len;
      this->pending_bytes_ += (next_key - next_buffer_index);
      capacity -= min(next_key - next_buffer_index,capacity);
    }
    else {
      this->buffer_.insert({next_buffer_index,
                          {sublen, data.substr(next_buffer_index - first_index, min(sublen,capacity))}});
      sublen = 0;
      this->pending_bytes_ += (sublen);
      capacity -= min(sublen,capacity);
    }
  }

 
  uint64_t cur_key = first_index;
  while(1){
    auto it = this->buffer_.begin();
    cur_key = it->first;
    if (cur_key != this->expected_next_indexed_){
      break;
    }
    this->output_.writer().push(it->second.second);
    this->pending_bytes_ -= it->second.first;
    this->expected_next_indexed_ += it->second.first;
    this->buffer_.erase(cur_key);
  }

  if (this-> expected_next_indexed_ == last_index && this->is_last_substring_){
    this->buffer_.clear();
  }


}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return this->pending_bytes_;
}
