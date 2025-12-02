/**
 * @file main.cpp
 * @brief Entry point for the fantasy combat simulation engine.
 *
 * This file initializes items, characters, strategies, and analytics systems,
 * then runs a simulated battle between two teams. It also demonstrates
 * pre-battle statistics, lore analysis, and turn-based combat behavior.
 */

#include <iostream>
#include <vector>
#include <ctime>

#include "Strategy.hpp"
#include "Item.hpp"
#include "Character.hpp"
#include "Analyzer.hpp"

/**
 * @brief Program entry point.
 *
 * Initializes all core gameplay components:
 * - Creates weapons, potions, and character inventories.
 * - Spawns two teams with different character classes and strategies.
 * - Performs pre-battle analytics:
 *   - Win chance estimation
 *   - Tankiest character detection
 * - Uses LoreAI to analyze setting descriptions.
 * - Simulates turn-based combat for up to 10 rounds.
 * - Declares a winner and awards XP.
 *
 * @return int Exit code (0 on success).
 */

int main() {
    std::srand(std::time(0));
    std::cout << "=== GAME ENGINE INITIALIZED ===\n\n";


     /// Sword: high damage, strong weapon
    auto sword = std::make_shared<Weapon>("Dragonslayer", 10.5, 15, 25);
     /// Axe: cheaper/ weaker starting weapon
    auto axe = std::make_shared<Weapon>("Rusty Axe", 8.0, 5, 10);
    /// Healing potion: restores 50 HP
    auto potion = std::make_shared<Potion>("Health Elixir", 0.5, 50);

    std::vector<std::shared_ptr<Character>> teamA;
    auto warrior = std::make_shared<Warrior>("Conan", std::make_unique<AggressiveStrategy>()); ///< Uses aggressive decision-making
    warrior->addItem(sword);
    teamA.push_back(warrior);

    std::vector<std::shared_ptr<Character>> teamB;
    auto mage = std::make_shared<Mage>("Merlin", std::make_unique<DefensiveStrategy>()); ///< Focuses on survival strategy
    teamB.push_back(mage);

    std::cout << "--- Pre-Battle Analytics ---\n";
     /**
     * @brief Calculates the chance for Team A to win.
     */
    double winChance = BattleSimulator<std::shared_ptr<Character>>::calculateWinChance(teamA, teamB);
    std::cout << "Team A Win Probability: " << winChance << "%\n";

     /**
     * @brief Detects which character in Team A has the most durability.
     */

    auto tank = BattleSimulator<std::shared_ptr<Character>>::findTankiest(teamA);
    std::cout << "Tankiest in Team A: " << tank->getName() << "\n\n";

    LoreAI ai;
    std::string description = "This is a dark hardcore fantasy game with dragons.";

    /**
     * @brief Extract metadata from textual game description.
     */
    auto meta = ai.analyzeDescription(description);
    std::cout << "--- Lore Analysis ---\n";
    std::cout << "Input: " << description << "\n";
    std::cout << "Detected Genre: " << meta.genre << "\n";
    std::cout << "Difficulty Rating: " << meta.difficultyRating << "/10\n\n";

    std::cout << "--- Battle Start ---\n";
    int rounds = 0;
    
    /**
     * @brief Simulates up to 10 turns of combat.
     *
     * Each character performs an action per round according to their strategy:
     * - Warriors use aggressive actions.
     * - Mages use defensive/cautious behavior.
     */
    while (warrior->isAlive() && mage->isAlive() && rounds < 10) {
        rounds++;
        std::cout << "Round " << rounds << ":\n";

        warrior->performTurn(mage.get());

        if (mage->isAlive()) {
            mage->performTurn(warrior.get());
        }

        std::cout << warrior->getName() << " HP: " << warrior->getHp() << " | "
                  << mage->getName() << " HP: " << mage->getHp() << "\n\n";
    }

    if (warrior->isAlive()) {
        warrior->gainXp(100);
        std::cout << "Winner: " << warrior->getName() << "\n";
    } else {
        std::cout << "Winner: " << mage->getName() << "\n";
    }

    return 0;
}