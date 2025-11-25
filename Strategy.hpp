#ifndef ANTONCHIK_STRATEGY_H
#define ANTONCHIK_STRATEGY_H

#include <iostream>
#include <string>

using namespace std;

class ICombatStrategy {
public:
    virtual ~ICombatStrategy() = default;

    virtual int decideAction(int currentHp, int maxHp, int enemyHp) const = 0;
    virtual std::string getName() const = 0;
};

class AggressiveStrategy : public ICombatStrategy {
public:
    std::string getName() const override { return "Berserker"; }

    int decideAction(int currentHp, int maxHp, int enemyHp) const override {
        if (enemyHp < 20) return 1;
        if ((double)currentHp / maxHp > 0.4) return 1;
        return (rand() % 2);
    }
};

class DefensiveStrategy : public ICombatStrategy {
public:
    std::string getName() const override { return "Tactician"; }

    int decideAction(int currentHp, int maxHp, int enemyHp) const override {
        if ((double)currentHp / maxHp < 0.3) {
            return 2;
        }
        return 0;
    }
};

#endif //ANTONCHIK_STRATEGY_H