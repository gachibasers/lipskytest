#ifndef ANTONCHIK_ITEM_H
#define ANTONCHIK_ITEM_H

#include <string>
#include <cstdlib>

class Item {
protected:
    std::string name;
    double weight;
    int condition;

public:
    Item(std::string n, double w) : name(n), weight(w), condition(100) {}
    virtual ~Item() = default;

    std::string getName() const { return name; }

    void repair(int amount) {
        condition += amount;
        if (condition > 100) condition = 100;
    }

    virtual int useEffect() = 0;
};

class Weapon : public Item {
private:
    int minDmg;
    int maxDmg;

public:
    Weapon(std::string n, double w, int min, int max)
        : Item(n, w), minDmg(min), maxDmg(max) {}

    int useEffect() override {
        if (condition <= 0) return 1; // Зламана зброя наносить 1 шкоди

        int base = minDmg + (rand() % (maxDmg - minDmg + 1));
        double conditionFactor = (double)condition / 100.0;

        condition -= 2;
        return static_cast<int>(base * conditionFactor);
    }
};

class Potion : public Item {
private:
    int healPower;
    bool isEmpty;

public:
    Potion(std::string n, double w, int power)
        : Item(n, w), healPower(power), isEmpty(false) {}

    int useEffect() override {
        if (isEmpty) return 0;
        isEmpty = true;
        return healPower;
    }
};

#endif //ANTONCHIK_ITEM_H