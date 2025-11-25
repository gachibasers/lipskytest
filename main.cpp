#include <iostream>
#include <vector>
#include <ctime>

#include "Strategy.hpp"
#include "Item.hpp"
#include "Character.hpp"
#include "Analyzer.hpp"

int main() {
    std::srand(std::time(0));
    std::cout << "=== GAME ENGINE INITIALIZED ===\n\n";

    auto sword = std::make_shared<Weapon>("Dragonslayer", 10.5, 15, 25);
    auto axe = std::make_shared<Weapon>("Rusty Axe", 8.0, 5, 10);
    auto potion = std::make_shared<Potion>("Health Elixir", 0.5, 50);

    std::vector<std::shared_ptr<Character>> teamA;
    auto warrior = std::make_shared<Warrior>("Conan", std::make_unique<AggressiveStrategy>());
    warrior->addItem(sword);
    teamA.push_back(warrior);

    std::vector<std::shared_ptr<Character>> teamB;
    auto mage = std::make_shared<Mage>("Merlin", std::make_unique<DefensiveStrategy>());
    teamB.push_back(mage);

    std::cout << "--- Pre-Battle Analytics ---\n";
    double winChance = BattleSimulator<std::shared_ptr<Character>>::calculateWinChance(teamA, teamB);
    std::cout << "Team A Win Probability: " << winChance << "%\n";

    auto tank = BattleSimulator<std::shared_ptr<Character>>::findTankiest(teamA);
    std::cout << "Tankiest in Team A: " << tank->getName() << "\n\n";

    LoreAI ai;
    std::string description = "This is a dark hardcore fantasy game with dragons.";
    auto meta = ai.analyzeDescription(description);
    std::cout << "--- Lore Analysis ---\n";
    std::cout << "Input: " << description << "\n";
    std::cout << "Detected Genre: " << meta.genre << "\n";
    std::cout << "Difficulty Rating: " << meta.difficultyRating << "/10\n\n";

    std::cout << "--- Battle Start ---\n";
    int rounds = 0;
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