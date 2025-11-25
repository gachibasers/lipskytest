#ifndef ANTONCHIK_ANALYZER_H
#define ANTONCHIK_ANALYZER_H

#include <vector>
#include <string>
#include <algorithm>

template <typename T>
class BattleSimulator {
public:
    static double calculateWinChance(const std::vector<T>& teamA, const std::vector<T>& teamB) {
        double hpA = 0, hpB = 0;
        for (const auto& unit : teamA) hpA += unit->getHp();
        for (const auto& unit : teamB) hpB += unit->getHp();

        if (hpA + hpB == 0) return 0.0;
        return (hpA / (hpA + hpB)) * 100.0;
    }

    static T findTankiest(const std::vector<T>& units) {
        if (units.empty()) return nullptr;

        T best = units[0];
        for (const auto& unit : units) {
            if (unit->getHp() > best->getHp()) {
                best = unit;
            }
        }
        return best;
    }
};

class LoreAI {
public:
    struct GameInfo {
        std::string genre;
        int difficultyRating;
    };

    GameInfo analyzeDescription(const std::string& desc) {
        GameInfo info;
        info.difficultyRating = 1;
        info.genre = "Unknown";

        std::string lowerDesc = desc;
        for(auto& c : lowerDesc) c = tolower(c);

        if (lowerDesc.find("dragon") != std::string::npos || lowerDesc.find("magic") != std::string::npos) {
            info.genre = "Fantasy RPG";
        }
        if (lowerDesc.find("gun") != std::string::npos) {
            info.genre = "Shooter";
        }

        if (lowerDesc.find("hardcore") != std::string::npos) info.difficultyRating += 5;
        if (lowerDesc.find("dark") != std::string::npos) info.difficultyRating += 2;
        if (lowerDesc.find("easy") != std::string::npos) info.difficultyRating -= 2;

        return info;
    }
};

#endif //ANTONCHIK_ANALYZER_H