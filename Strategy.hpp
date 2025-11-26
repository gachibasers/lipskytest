#ifndef ANTONCHIK_STRATEGY_H
#define ANTONCHIK_STRATEGY_H

#include <iostream>
#include <string>

using namespace std;
/**
 * @brief Interface for all combat strategies.
 *
 * A strategy defines how a character behaves during battle. Each implementation
 * decides which action to take based on current HP, maximum HP, and the enemy's HP.
 */
class ICombatStrategy {
public:
    virtual ~ICombatStrategy() = default;
    /**
     * @brief Determines the action a character performs during their turn.
     *
     * Possible returned values depend on your game design:
     * - 0 — basic attack
     * - 1 — special/aggressive attack
     * - 2 — defensive or healing action
     *
     * @param currentHp Current HP of the character.
     * @param maxHp Maximum HP of the character.
     * @param enemyHp Current HP of the enemy.
     * @return Integer code representing chosen action.
     */
    virtual int decideAction(int currentHp, int maxHp, int enemyHp) const = 0;
    virtual std::string getName() const = 0;
};
/**
 * @brief Aggressive "Berserker" strategy.
 *
 * A character using this strategy tends to attack aggressively most of the time.
 * Behavior rules:
 * - If the enemy HP < 20 → always attack aggressively.
 * - If own HP > 40% → aggressive attack.
 * - Otherwise → randomly choose between normal and aggressive attack.
 */
class AggressiveStrategy : public ICombatStrategy {
public:
    std::string getName() const override { return "Berserker"; }
   /**
     * @return Strategy name: "Berserker".
     */
    int decideAction(int currentHp, int maxHp, int enemyHp) const override {
        if (enemyHp < 20) return 1;
        if ((double)currentHp / maxHp > 0.4) return 1;
        return (rand() % 2);
    }
};

class DefensiveStrategy : public ICombatStrategy {
public:
    std::string getName() const override { return "Tactician"; }
     /**
     * @brief Chooses defensive action if health is critically low.
     */
    int decideAction(int currentHp, int maxHp, int enemyHp) const override {
        if ((double)currentHp / maxHp < 0.3) {
            return 2;
        }
        return 0;
    }
};

#endif //ANTONCHIK_STRATEGY_H