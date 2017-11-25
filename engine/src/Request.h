#pragma once

#include "Common.h"
#include "RequestType.pb.h"
#include <iostream>
#include <memory>

struct Request {
  RequestType m_type;
  uint32 m_length;
  std::unique_ptr<unsigned char[]> m_data;

  enum Header { SIZE = 8 };
};

std::ostream& operator<<(std::ostream& out, const Request& request);
