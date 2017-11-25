#include "Request.h"

#include <google/protobuf/descriptor.h>

std::ostream& operator<<(std::ostream& out, const Request& request) {
  auto descriptor = RequestType_descriptor()->FindValueByNumber(request.m_type);
  if (descriptor) {
    out << descriptor->name();
  } else {
    out << "Unknown Type";
  }
  out << " [" << request.m_length << "]";
  if (request.m_data) {
    out << " ";
    out.write((char*)request.m_data.get(), request.m_length);
  }
  return out;
}
