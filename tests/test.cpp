#include <gtest/gtest.h>
#include <memory>
#include <cmath>
#include <vector>

#include "Strategy.hpp"
#include "Item.hpp"
#include "Character.hpp"
#include "Analyzer.hpp"

TEST(ItemTest, WeaponDamageRange) {
    auto sword = std::make_shared<Weapon>("Test Sword", 1.0, 10, 20);
    int damage = sword->useEffect();
    ASSERT_GE(damage, 10);
    ASSERT_LE(damage, 20);
}

TEST(ItemTest, WeaponBreaking) {
    auto stick = std::make_shared<Weapon>("Stick", 1.0, 10, 10);
    for(int i=0; i < 51; ++i) {
        stick->useEffect();
    }
    ASSERT_EQ(stick->useEffect(), 1);
}

TEST(ItemTest, PotionHealing) {
    auto pot = std::make_shared<Potion>("Heal", 0.5, 50);
    ASSERT_EQ(pot->useEffect(), 50);
}

TEST(ItemTest, PotionOneTimeUse) {
    auto pot = std::make_shared<Potion>("Heal", 0.5, 50);
    pot->useEffect();
    ASSERT_EQ(pot->useEffect(), 0);
}

TEST(StrategyTest, AggressiveStrategyAttackLowEnemyHP) {
    AggressiveStrategy strat;
    int action = strat.decideAction(100, 100, 10);
    ASSERT_EQ(action, 1);
}

TEST(StrategyTest, DefensiveStrategyHealLowSelfHP) {
    DefensiveStrategy strat;
    int action = strat.decideAction(20, 100, 100);
    ASSERT_EQ(action, 2);
}

TEST(CharacterTest, CharacterTakingDamage) {
    auto dummy = std::make_shared<Warrior>("Dummy", std::make_unique<DefensiveStrategy>());
    int startHp = dummy->getHp();
    dummy->takeDamage(50);
    ASSERT_EQ(dummy->getHp(), startHp - 50);
}

TEST(CharacterTest, CharacterDeath) {
    auto dummy = std::make_shared<Warrior>("DeadMan", std::make_unique<DefensiveStrategy>());
    dummy->takeDamage(1000);
    ASSERT_FALSE(dummy->isAlive());
    ASSERT_EQ(dummy->getHp(), 0);
}

TEST(CharacterTest, LevelUpMechanics) {
    auto hero = std::make_shared<Warrior>("Hero", std::make_unique<AggressiveStrategy>());
    int initialHp = hero->getHp();

    hero->takeDamage(50);
    hero->gainXp(100);
    ASSERT_GT(hero->getHp(), initialHp);
}

TEST(CharacterTest, InventoryUsageDamage) {
    auto w = std::make_shared<Warrior>("Conan", std::make_unique<AggressiveStrategy>());
    auto sword = std::make_shared<Weapon>("Sword", 5.0, 20, 20);
    w->addItem(sword);

    auto target = std::make_shared<Mage>("Target", std::make_unique<DefensiveStrategy>());
    int hpBefore = target->getHp();

    w->performTurn(target.get());

    int dmgDealt = hpBefore - target->getHp();

    ASSERT_GE(dmgDealt, 20);
}

TEST(AnalyzerTest, WinProbabilityCalculation) {
    std::vector<std::shared_ptr<Character>> teamA;
    std::vector<std::shared_ptr<Character>> teamB;

    auto c1 = std::make_shared<Warrior>("A", std::make_unique<AggressiveStrategy>());
    teamA.push_back(c1);

    double chance = BattleSimulator<std::shared_ptr<Character>>::calculateWinChance(teamA, teamB);

    ASSERT_NEAR(chance, 100.0, 0.001);
}

TEST(AnalyzerTest, FindTankiest) {
    std::vector<std::shared_ptr<Character>> team;
    auto weak = std::make_shared<Mage>("Weak", std::make_unique<DefensiveStrategy>()); // 80 HP
    auto strong = std::make_shared<Warrior>("Strong", std::make_unique<AggressiveStrategy>()); // 150 HP

    team.push_back(weak);
    team.push_back(strong);

    auto best = BattleSimulator<std::shared_ptr<Character>>::findTankiest(team);
    ASSERT_EQ(best->getName(), "Strong");
}

TEST(LoreAITest, LoreGenreDetection) {
    LoreAI ai;
    auto meta = ai.analyzeDescription("A game with a Dragon and magic spells.");
    ASSERT_EQ(meta.genre, "Fantasy RPG");
}

TEST(LoreAITest, LoreDifficultyCalculation) {
    LoreAI ai;
    auto meta = ai.analyzeDescription("Dark Hardcore souls-like game.");
    ASSERT_EQ(meta.difficultyRating, 8);
}