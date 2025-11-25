#ifndef ANTONCHIK_CHARACTER_H
#define ANTONCHIK_CHARACTER_H

#include <vector>
#include <memory>
#include <cmath>
#include "Item.hpp"
#include "Strategy.hpp"

class Entity {
public:
    virtual void update() = 0;
    virtual ~Entity() = default;
};

class Character : public Entity {
protected:
    std::string name;
    int hp;
    int maxHp;
    int xp;
    int level;
    std::vector<std::shared_ptr<Item>> inventory;
    std::unique_ptr<ICombatStrategy> ai;

public:
    Character(std::string n, int h, std::unique_ptr<ICombatStrategy> s)
        : name(n), hp(h), maxHp(h), xp(0), level(1), ai(std::move(s)) {}

    bool isAlive() const { return hp > 0; }
    std::string getName() const { return name; }
    int getHp() const { return hp; }

    void addItem(std::shared_ptr<Item> it) {
        inventory.push_back(it);
    }

    void gainXp(int amount) {
        xp += amount;
        int threshold = level * 50;
        if (xp >= threshold) {
            level++;
            xp -= threshold;
            maxHp = static_cast<int>(maxHp * 1.1); // +10% HP
            hp = maxHp;
            std::cout << ">>> " << name << " LEVEL UP! (Lvl " << level << ")\n";
        }
    }

    void takeDamage(int dmg) {
        hp -= dmg;
        if (hp < 0) hp = 0;
    }

    virtual int performTurn(Character* target) = 0;
};

class Warrior : public Character {
    int rage;
public:
    Warrior(std::string n, std::unique_ptr<ICombatStrategy> s)
        : Character(n, 150, std::move(s)), rage(0) {}

    void update() override { }

    int performTurn(Character* target) override {
        int action = ai->decideAction(hp, maxHp, target->getHp());
        int dmg = 0;
        for(auto& item : inventory) {
            if(auto w = std::dynamic_pointer_cast<Weapon>(item)) {
                dmg += w->useEffect();
            }
        }
        if (dmg == 0) dmg = 2;

        if (action == 1) {
            if (rage > 30) {
                dmg *= 2; rage = 0;
                std::cout << name << " CRITICAL SMASH ";
            } else {
                rage += 10;
                std::cout << name << " attacks ";
            }
            target->takeDamage(dmg);
            return dmg;
        } else {
            rage -= 5;
            std::cout << name << " holds position.\n";
            return 0;
        }
    }
};

class Mage : public Character {
    int mana;
public:
    Mage(std::string n, std::unique_ptr<ICombatStrategy> s)
        : Character(n, 80, std::move(s)), mana(100) {}

    void update() override { mana += 5; }

    int performTurn(Character* target) override {
        int action = ai->decideAction(hp, maxHp, target->getHp());

        if (action == 2) {
            if (mana >= 20) {
                hp += 30; mana -= 20;
                if (hp > maxHp) hp = maxHp;
                std::cout << name << " heals self.\n";
            }
            return 0;
        }

        if (mana >= 15) {
            int dmg = 25 + (rand() % 10);
            mana -= 15;
            target->takeDamage(dmg);
            std::cout << name << " casts Fireball dealing " << dmg << " dmg.\n";
            return dmg;
        } else {
            std::cout << name << " meditates (No Mana).\n";
            mana += 20;
            return 0;
        }
    }
};

#endif //ANTONCHIK_CHARACTER_H