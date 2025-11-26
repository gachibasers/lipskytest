#ifndef ANTONCHIK_ITEM_H
#define ANTONCHIK_ITEM_H

#include <string>
#include <cstdlib>

/**
 * @brief Базовий абстрактний клас для всіх предметів гри.
 *
 * Містить:
 * - назву предмета
 * - вагу
 * - стан (0–100)
 *
 * Кожен предмет повинен реалізувати метод useEffect().
 */
class Item {
protected:
    std::string name;   ///< Назва предмета.
    double weight;      ///< Вага предмета.
    int condition;      ///< Стан предмета (0–100).

public:

    /**
     * @brief Створює новий предмет.
     * @param n Назва
     * @param w Вага
     */
    Item(std::string n, double w) : name(n), weight(w), condition(100) {}

    virtual ~Item() = default;

    /**
     * @brief Повертає назву предмета.
     */
    std::string getName() const { return name; }

    /**
     * @brief Ремонтує предмет, збільшуючи його стан.
     *
     * @param amount Значення, на яке збільшується стан.
     * Стан не може перевищувати 100.
     */
    void repair(int amount) {
        condition += amount;
        if (condition > 100) condition = 100;
    }

    /**
     * @brief Використання предмета.
     *
     * Конкретна реалізація залежить від типу предмета:
     * - Weapon → наносить шкоду
     * - Potion → лікує
     *
     * @return Значення ефекту від використання.
     */
    virtual int useEffect() = 0;
};

/**
 * @brief Зброя, яка завдає шкоди залежно від стану.
 *
 * Має мінімальну та максимальну шкоду.
 * Стан зброї зменшується на 2 за кожне використання.
 */
class Weapon : public Item {
private:
    int minDmg; ///< Мінімальна шкода.
    int maxDmg; ///< Максимальна шкода.

public:

    /**
     * @brief Створює зброю.
     * @param n   Назва зброї
     * @param w   Вага
     * @param min Мінімальна шкода
     * @param max Максимальна шкода
     */
    Weapon(std::string n, double w, int min, int max)
        : Item(n, w), minDmg(min), maxDmg(max) {}

    /**
     * @brief Використання зброї.
     *
     * Обчислюється випадкова шкода в діапазоні [minDmg, maxDmg].
     * Стан зменшується на 2.
     * Якщо стан ≤ 0 → зброя завдає 1 шкоди.
     *
     * @return Завдана шкода.
     */
    int useEffect() override {
        if (condition <= 0) return 1; ///< Зламана зброя.

        int base = minDmg + (rand() % (maxDmg - minDmg + 1));
        double conditionFactor = (double)condition / 100.0;

        condition -= 2;
        return static_cast<int>(base * conditionFactor);
    }
};

/**
 * @brief Зілля для лікування.
 *
 * Одноразовий предмет: після використання стає порожнім.
 */
class Potion : public Item {
private:
    int healPower; ///< Кількість HP, яке відновлює зілля.
    bool isEmpty;  ///< Чи вже використане.

public:

    /**
     * @brief Створює зілля.
     * @param n     Назва
     * @param w     Вага
     * @param power Сила лікування
     */
    Potion(std::string n, double w, int power)
        : Item(n, w), healPower(power), isEmpty(false) {}

    /**
     * @brief Використання зілля.
     *
     * Якщо зілля вже використане → повертає 0.
     * Після першого використання зілля спустошується.
     *
     * @return Відновлені HP.
     */
    int useEffect() override {
        if (isEmpty) return 0;
        isEmpty = true;
        return healPower;
    }
};

#endif //ANTONCHIK_ITEM_H
