#ifndef ANTONCHIK_CHARACTER_H
#define ANTONCHIK_CHARACTER_H

#include <vector>
#include <memory>
#include <cmath>
#include "Item.hpp"
#include "Strategy.hpp"

/**
 * @brief Базовий інтерфейс для всіх оновлюваних сутностей гри.
 */
class Entity {
public:
    /**
     * @brief Оновлює стан об'єкта (викликається кожен хід).
     */
    virtual void update() = 0;

    virtual ~Entity() = default;
};

/**
 * @brief Базовий клас ігрового персонажа.
 *
 * Містить:
 * - здоровʼя, рівень, досвід
 * - інвентар з предметами
 * - бойову стратегію (AI)
 */
class Character : public Entity {
protected:
    std::string name;                           ///< Імʼя персонажа.
    int hp;                                     ///< Поточні очки здоровʼя.
    int maxHp;                                  ///< Максимальні очки здоровʼя.
    int xp;                                     ///< Досвід персонажа.
    int level;                                  ///< Поточний рівень.
    std::vector<std::shared_ptr<Item>> inventory; ///< Інвентар.
    std::unique_ptr<ICombatStrategy> ai;        ///< Вибрана стратегія бою (AI).

public:

    /**
     * @brief Створює персонажа з імʼям, HP та AI.
     *
     * @param n   Імʼя
     * @param h   Початкове HP (також максимальне)
     * @param s   Унікальний вказівник на бойову стратегію
     */
    Character(std::string n, int h, std::unique_ptr<ICombatStrategy> s)
        : name(n), hp(h), maxHp(h), xp(0), level(1), ai(std::move(s)) {}

    /**
     * @brief Перевіряє чи живий персонаж.
     */
    bool isAlive() const { return hp > 0; }

    /**
     * @brief Повертає імʼя персонажа.
     */
    std::string getName() const { return name; }

    /**
     * @brief Повертає поточні очки здоровʼя.
     */
    int getHp() const { return hp; }

    /**
     * @brief Додає предмет у інвентар.
     */
    void addItem(std::shared_ptr<Item> it) {
        inventory.push_back(it);
    }

    /**
     * @brief Додає досвід та підвищує рівень при досягненні порогу.
     *
     * @param amount Кількість отриманого досвіду
     */
    void gainXp(int amount) {
        xp += amount;
        int threshold = level * 50;
        if (xp >= threshold) {
            level++;
            xp -= threshold;
            maxHp = static_cast<int>(maxHp * 1.1); ///< +10% до максимального HP
            hp = maxHp;
            std::cout << ">>> " << name << " LEVEL UP! (Lvl " << level << ")\n";
        }
    }

    /**
     * @brief Застосовує пошкодження до персонажа.
     *
     * @param dmg Кількість шкоди
     */
    void takeDamage(int dmg) {
        hp -= dmg;
        if (hp < 0) hp = 0;
    }

    /**
     * @brief Виконує хід у бою.
     *
     * **Абстрактний метод**, реалізується у класах Warrior та Mage.
     *
     * @param target Вказівник на ціль персонажа
     * @return Нанесена шкода
     */
    virtual int performTurn(Character* target) = 0;
};

/**
 * @brief Клас воїна — танк/меле-боєць з системою "люті" (rage).
 */
class Warrior : public Character {
    int rage; ///< Поточний рівень люті.

public:
    /**
     * @brief Створює воїна.
     */
    Warrior(std::string n, std::unique_ptr<ICombatStrategy> s)
        : Character(n, 150, std::move(s)), rage(0) {}

    /**
     * @brief Оновлення стану (у даному випадку — нічого не робить).
     */
    void update() override { }

    /**
     * @brief Виконує бойовий хід.
     *
     * Воїн:
     * - накопичує лють за атаки
     * - при 30+ люті наносить критичний удар
     * - використовує всі предмети типу Weapon з інвентаря
     *
     * @param target Ціль атаки
     * @return Нанесена шкода
     */
    int performTurn(Character* target) override {
        int action = ai->decideAction(hp, maxHp, target->getHp());
        int dmg = 0;

        for (auto& item : inventory) {
            if (auto w = std::dynamic_pointer_cast<Weapon>(item)) {
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

/**
 * @brief Клас мага — герой з маною та магічними атаками.
 */
class Mage : public Character {
    int mana; ///< Поточна кількість мани.

public:
    /**
     * @brief Створює мага з початковою маною 100.
     */
    Mage(std::string n, std::unique_ptr<ICombatStrategy> s)
        : Character(n, 80, std::move(s)), mana(100) {}

    /**
     * @brief Оновлення стану — кожен хід додає 5 мани.
     */
    void update() override { mana += 5; }

    /**
     * @brief Виконує хід:
     * - лікує себе якщо AI повертає 2
     * - застосовує магічну атаку якщо є мана
     * - медитує якщо мани недостатньо
     *
     * @param target Ціль
     * @return Нанесена шкода
     */
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