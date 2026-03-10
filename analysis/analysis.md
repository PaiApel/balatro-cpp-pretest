# Design Pattern Analysis

## PART A — Creational Pattern Analysis

### A1 — Factory

**Where are modifier objects created?**

Modifier objects are created inside `ModifierFactory::createModifier()` in `src/ModifierFactory.cpp`. This static method accepts a `std::string` type identifier and returns a pointer to a newly allocated `IModifier` object.

```cpp
IModifier* ModifierFactory::createModifier(const std::string& type) {
    if (type == "DoubleTrouble") {
	return new DoubleTrouble();
    }
    if (type == "LuckySeven") {
	return new LuckySeven();
    }
    // ... and so on for all 11 modifiers
}
```

**Which class creates them?**

The `ModifierFactory` class is the sole creator. No other class calls `new` on any concrete modifier. `ShopSystem` calls `ModifierFactory::createModifier()` when the player purchases an item in `ShopSystem::buyItem()` in `src/ShopSystem.cpp`.

**Why centralizing creation is beneficial:**

Without a factory, every system that needs a modifier would need to `#include` every concrete modifier header and call `new DoubleTrouble()` directly. This creates tight coupling. With `ModifierFactory`, only one file knows about concrete modifier types. Adding a new modifier only requires updating `ModifierFactory.cpp` and `ShopSystem.cpp` — no other class needs to change.

---

### A2 — Extensibility

**How was SquareScoreModifier added?**

To add `SquareScoreModifier` to the system, only the following files were changed or created:

1. **Created** `src/modifiers/SquareScoreModifier.h` — declared the class inheriting from `IModifier`
2. **Created** `src/modifiers/SquareScoreModifier.cpp` — implemented `apply()`, `getName()`, `getDescription()`, `getCost()`
3. **Updated** `src/ModifierFactory.cpp` — added `#include "modifiers/SquareScoreModifier.h"` and one `if` branch in `createModifier()`
4. **Updated** `src/ShopSystem.cpp` — added one entry in `buildItemPool()`

No changes were needed in `RunSession`, `ScoringSystem`, `IModifier`, or any other modifier. The system was easy to extend precisely because of the Factory pattern and the `IModifier` interface.

**Was the design easy to extend?**

Yes. The `IModifier` interface enforces a consistent contract (`apply()`, `getName()`, `getDescription()`, `getCost()`), so any new modifier only needs to implement those four methods. The factory and shop pool are the only two places that need updating.

---

### A3 — Factory Method (Conceptual)

If the game had multiple shop types (e.g., a card shop, a modifier shop, a relic shop), the Factory Method pattern would help by defining a common interface for shop creation while letting subclasses decide which products to create.

Example interface:

```cpp
class IShopFactory {
public:
    virtual IShopItem* createItem(const std::string& type) = 0;
    virtual ~IShopFactory() {}
};

class ModifierShopFactory : public IShopFactory {
public:
    IShopItem* createItem(const std::string& type) override {
        return ModifierFactory::createModifier(type);
    }
};

class CardShopFactory : public IShopFactory {
public:
    IShopItem* createItem(const std::string& type) override {
        // return new card item
    }
};
```

`RunSession` would hold an `IShopFactory*` and call `createItem()` without knowing which shop type it is, allowing easy swapping of shop behavior.

---

### A4 — Prototype (Conceptual)

The Prototype pattern could be used to duplicate modifiers — useful if the player can copy an existing modifier they own, or if the shop needs to offer "copies" of modifiers without re-creating them from scratch.

`Clone()` would be added to `IModifier`:

```cpp
class IModifier {
public:
    virtual IModifier* clone() const = 0;
    // ... existing methods
};
```

Each concrete modifier would implement it:

```cpp
IModifier* DoubleTrouble::clone() const {
    return new DoubleTrouble(*this);
}
```

In the current codebase, `clone()` would be implemented in each file under `src/modifiers/` such as `DoubleTrouble.cpp`, `LuckySeven.cpp`, etc. This would be most useful in `ShopSystem::buyItem()` in `src/ShopSystem.cpp`, where instead of calling `ModifierFactory::createModifier()`, the shop could clone a pre-built prototype.

---

### A5 — Singleton (Conceptual)

The current implementation does not use global objects. `RunSession`, `ScoringSystem`, `ShopSystem`, and `ModifierFactory` are all instantiated locally — `ScoringSystem` as a member of `RunSession`, and `ShopSystem` as a pointer created in `RunSession::startRun()`.

**Advantages of Singleton** (if applied to e.g. `ModifierFactory`):
- Guarantees one shared instance across the entire program
- Avoids passing the factory reference through multiple layers

**Risks:**
- Hidden global state makes testing harder
- Tight coupling — any class can access the singleton, making dependencies less clear
- Thread safety issues in multi-threaded contexts

In this project, since `ModifierFactory` only has static methods, a Singleton is unnecessary — it already behaves like one without the downsides.

---

## PART B — Structural Pattern Analysis

### B1 — Decorator Concept

The modifier system behaves like the Decorator pattern. In classic Decorator, a base object is "wrapped" by decorator objects that add behavior. Here, `ScoreContext` (in `src/ScoreContext.h`) is the object being decorated — it holds `chips` and `mult` and computes `finalScore()`.

Each modifier wraps the scoring process by modifying `ScoreContext` before the final score is computed:

```cpp
// In ScoringSystem::calculate() — src/ScoringSystem.cpp
for (IModifier* m : modifiers) {
    m->apply(ctx);
}
```

For example, `DoubleTrouble::apply()` in `src/modifiers/DoubleTrouble.cpp` modifies the context:

```cpp
void DoubleTrouble::apply(ScoreContext& context) {
    context.mult *= 2;
}
```

Each modifier adds a "layer" of behavior on top of the base score, exactly like Decorator wraps an object with additional functionality.

**What is being decorated?**

The `ScoreContext` object — specifically its `chips` and `mult` values — is being decorated by each `IModifier` in sequence.

---

### B2 — Modifier Order

Yes, modifier order significantly affects the final score. The modifiers are stored in `std::vector<IModifier*> activeModifiers` in `RunSession` and applied in order in `ScoringSystem::calculate()`.

Example using actual modifiers:

Suppose base score is Chips: 50, Mult: 2.

**Order A: MegaPayout → LuckySeven → DoubleTrouble**
- MegaPayout: Chips 50 + 90 = 140, Mult 2
- LuckySeven: Chips 140, Mult 9
- DoubleTrouble: Chips 140, Mult 18
- Final: 140 × 16 = **2520**

**Order B: DoubleTrouble → LuckySeven → MegaPayout**
- DoubleTrouble: Chips 50, Mult 4
- LuckySeven: Chips 50, Mult 11
- MegaPayout: Chips 140, Mult 11
- Final: 140 × 8 = **1540**

Same modifiers, different order, different final score. This is why the game provides a modifier reordering feature via `ShopSystem::openModifierManager()` in `src/ShopSystem.cpp`.

---

### B3 — Composite (Conceptual)

A modifier group (Composite) would allow treating a collection of modifiers as a single modifier. For example, a "Combo Pack" that contains DoubleTrouble + LuckySeven and applies both as one unit.

```cpp
class ModifierGroup : public IModifier {
private:
    std::vector<IModifier*> modifiers;
public:
    void add(IModifier* m) { modifiers.push_back(m); }
    void apply(ScoreContext& context) override {
        for (IModifier* m : modifiers)
            m->apply(context);
    }
    std::string getName() const override { return "Modifier Group"; }
};
```

This is useful for bundle deals in the shop or for applying preset modifier configurations.

---

### B4 — Adapter (Conceptual)

If an external scoring library was integrated (e.g., a third-party poker hand evaluator that returns a `float` score), its interface would differ from our `ScoreContext`-based system. An Adapter would bridge the gap:

```cpp
class ExternalScoringAdapter : public ScoringSystem {
private:
    ExternalScoringLib* externalLib;
public:
    ScoreContext calculate(const std::vector<Card>& cards, const std::vector<IModifier*>& modifiers) override {
        float externalScore = externalLib->evaluate(cards);
        int chips = static_cast<int>(externalScore);
        return ScoreContext(chips, 1);
    }
};
```

This would allow `RunSession` to use the external system without any changes to `RunSession.cpp` or `ScoringSystem.h`.

---

### B5 — Facade (Conceptual)

A `GameFacade` class would expose a simplified interface to the entire game system, hiding the complexity of `RunSession`, `ScoringSystem`, `ShopSystem`, and `ModifierFactory`.

```cpp
class GameFacade {
public:
    void startGame();
    void playHand(const std::vector<int>& cardIndices);
    void discardCards(const std::vector<int>& cardIndices);
    void openShop();
    void buyModifier(int shopIndex);
    void sellModifier(int modifierIndex);
    int getCurrentScore() const;
    int getGold() const;
};
```

In the current codebase, `RunSession` already acts as an informal Facade — it coordinates `Deck`, `ScoringSystem`, `ShopSystem`, and the modifier list. A formal `GameFacade` would make this role explicit and make it easier to add a GUI or AI player on top.

---

## PART C — Behavioral Pattern Analysis

### C1 — Strategy

The scoring logic lives in `HandEvaluator::evaluate()` in `src/HandEvaluator.cpp`. It detects the hand type and returns base chips and mult from the static `handDefinitions` table. `ScoringSystem::calculate()` in `src/ScoringSystem.cpp` calls this and applies modifiers.

**How to support multiple scoring rules:**

The Strategy pattern would allow swapping the scoring algorithm at runtime:

```cpp
class IScoringStrategy {
public:
    virtual HandEvaluator::HandResult evaluate(const std::vector<Card>& cards) = 0;
    virtual ~IScoringStrategy() {}
};

class StandardScoringStrategy : public IScoringStrategy {
public:
    HandEvaluator::HandResult evaluate(const std::vector<Card>& cards) override {
        return HandEvaluator::evaluate(cards);
    }
};

class HighCardOnlyScoringStrategy : public IScoringStrategy {
    // Always treats hand as High Card regardless of actual hand
};
```

`ScoringSystem` would hold an `IScoringStrategy*` and call it in `calculate()`. This would allow game modes like "only pairs count" or "flushes are disabled" without changing any other code.

---

### C2 — Observer (Conceptual)

Several events occur naturally in the game loop that other systems might want to react to:

- **Round cleared** — trigger gold reward calculation, shop refresh
- **Modifier bought** — refresh modifier display
- **Score calculated** — update round total display
- **Round failed** — trigger game over sequence

With Observer, these events would be broadcast to registered listeners:

```cpp
class IGameObserver {
public:
    virtual void onRoundCleared(int score, int gold) = 0;
    virtual void onModifierBought(IModifier* modifier) = 0;
    virtual void onGameOver(int finalRound) = 0;
};
```

Currently `RunSession` handles all of this directly in `playRound()` and `startRun()`. Observer would decouple these reactions, making it easier to add features like an achievement system or statistics tracker without modifying `RunSession`.

---

### C3 — Command (Conceptual)

Player actions in the game are currently handled inline in `RunSession::playRound()` using the `PlayerAction` struct. These could be represented as Command objects:

```cpp
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class PlayHandCommand : public ICommand {
    std::vector<Card> selectedCards;
    RunSession& session;
public:
    void execute() override { session.scoreHand(selectedCards); }
    void undo() override { session.undoLastHand(); }
};

class DiscardCommand : public ICommand {
    std::vector<Card> toDiscard;
public:
    void execute() override { /* remove cards, draw new ones */ }
    void undo() override { /* restore discarded cards */ }
};
```

Command would enable undo/redo functionality — useful for an "undo last play" feature — and would make it easier to log or replay game sessions.

---

### C4 — State (Conceptual)

The game currently moves through distinct phases managed by flags and conditionals in `RunSession::startRun()` and `RunSession::playRound()`:

- **Playing** — player is selecting and playing cards
- **Shopping** — player is in the shop between rounds
- **GameOver** — run has ended

The State pattern would represent each phase as a class:

```cpp
class IGameState {
public:
    virtual void update(RunSession& session) = 0;
    virtual ~IGameState() {}
};

class PlayingState : public IGameState {
public:
    void update(RunSession& session) override {
        // handle hand selection, scoring
    }
};

class ShoppingState : public IGameState {
public:
    void update(RunSession& session) override {
        // handle shop interaction
    }
};

class GameOverState : public IGameState {
public:
    void update(RunSession& session) override {
        // display game over screen
    }
};
```

`RunSession` would hold an `IGameState*` and call `state->update(*this)` in its main loop. Transitions between states would be explicit and self-contained, removing the need for `gameOver` flags and nested conditionals.
