#pragma once

#include <ostream>
#include <map>
#include <string>
#include "Common.h"


struct Stats {
  std::map<int32, int32> m_finishPlaceCounts;
  std::vector<int32> m_scores;
};

class StatTracker {
public:
  static StatTracker& get();

  StatTracker(const StatTracker& other) = delete;
  StatTracker& operator=(const StatTracker& other) = delete;
  std::map<std::string, std::map<Stats>> m_aiStats;

private:
  StatTracker();
};

std::ostream& operator<<(std::ostream& out, StatTracker& statTracker);
