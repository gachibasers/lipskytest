#ifndef ANTONCHIK_ANALYZER_H
#define ANTONCHIK_ANALYZER_H

#include <vector>
#include <string>
#include <algorithm>
/**
 * @brief A generic battle simulation helper.
 *
 * This template class provides utility methods for analyzing teams of characters.
 * It is intended to work with smart pointers such as std::shared_ptr<Character>.
 *
 * @tparam T A type representing a character or unit pointer
 *           (e.g., std::shared_ptr<Character>).
 */
template <typename T>
class BattleSimulator {
public:

    /**
     * @brief Calculates Team A's win probability based on total HP.
     *
     * The formula used is:
     * \f[
     *   \text{WinChance} = \frac{\sum HP_A}{\sum HP_A + \sum HP_B} \times 100
     * \f]
     *
     * @param teamA A vector of units (pointers) representing the first team.
     * @param teamB A vector of units (pointers) representing the second team.
     * @return Percentage value from 0 to 100 representing win probability.
     *
     * @note If both teams have 0 total HP, returns 0.
     */
    static double calculateWinChance(const std::vector<T>& teamA, const std::vector<T>& teamB) {
        double hpA = 0, hpB = 0;
        for (const auto& unit : teamA) hpA += unit->getHp();
        for (const auto& unit : teamB) hpB += unit->getHp();

        if (hpA + hpB == 0) return 0.0;
        return (hpA / (hpA + hpB)) * 100.0;
    }
      }

    /**
     * @brief Finds the tankiest (highest HP) unit in a team.
     *
     * Iterates over all units and selects the one with the greatest HP.
     *
     * @param units Vector of units to evaluate.
     * @return The pointer T to the tankiest unit, or nullptr if the list is empty.
     */

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
 /**
     * @brief Analyzes a textual description of a game and returns genre metadata.
     *
     * The parser performs case-insensitive keyword matching:
     * - "dragon" or "magic" → Fantasy RPG
     * - "gun" → Shooter
     *
     * Difficulty modifiers:
     * - "hardcore" → +5
     * - "dark" → +2
     * - "easy" → -2
     *
     * @param desc Input description string.
     * @return A populated GameInfo struct.
     *
     * @note Difficulty starts at 1 and can only increase but not exceed logical bounds in real use.
     * @warning Not AI — just heuristic keyword s
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