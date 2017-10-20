#include "StatTracker.h"

StatTracker& StatTracker::get() {
  static StatTracker instance;
  return instance;
}

StatTracker::StatTracker() {
}

std::ostream& operator<<(std::ostream& out, StatTracker& statTracker) {
  for (auto& aiKv : statTracker.m_aiStats) {
    out << aiKv.first << "\n";
    for (auto& playerCountKv : aiKv.second) {
      out << "  " << playerCountKv.first << " player games\n";
      int64 totalScore = 0;
      for (auto score : playerCountKv.second.m_scores) {
        totalScore += score;
      }
      out << "    " << totalScore << " points after " << playerCountKv.second.m_scores.size() << " game(s).\n";
      for (int32 i = 1; i <= playerCountKv.first; i++) {
        playerCountKv.second.m_finishPlaceCounts[i] += 0;
      }
      for (auto& placeCountsKv : playerCountKv.second.m_finishPlaceCounts) {
        out << "    Finished (" << placeCountsKv.first << "/" << playerCountKv.first << ") " << placeCountsKv.second
            << " time(s).\n";
      }
    }
  }
  return out;
}
