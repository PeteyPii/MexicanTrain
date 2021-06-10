#pragma once

#include "Common.h"
#include <map>
#include <ostream>
#include <string>
#include <vector>

struct Stats {
  std::map<int32, int32> m_finishPlaceCounts;
  std::vector<int32> m_scores;
};

class StatTracker {
public:
  static StatTracker& get();

  StatTracker(const StatTracker& other) = delete;
  StatTracker& operator=(const StatTracker& other) = delete;
  std::map<std::string, std::map<int32, Stats>> m_aiStats;

private:
  StatTracker();
};

std::ostream& operator<<(std::ostream& out, StatTracker& statTracker);
