A text-based card roguelike game built in C++ as a design patterns demonstration.
Inspired by the genre of Balatro, the player plays poker hands to score points, buys modifiers to boost their score, and tries to survive as many rounds as possible.

---

## How to Play

```
[numbers]     = play selected cards (e.g. 1 3 5)
[numbers] D   = discard selected cards (e.g. 2 4 D)
RS            = sort hand by rank
SS            = sort hand by suit
H             = show poker hand reference
Q             = quit run (asks for confirmation)
```

In the shop:
```
[number]  = buy item
M         = manage modifiers (swap, sell)
R         = reroll shop (costs gold, increases each reroll)
0         = leave shop
```

In modifier manager:
```
1 3       = swap modifier positions
S 2       = sell modifier 2
0         = back to shop
```

---

## Architecture Overview

### `Card` / `Deck` — `src/Card.h/.cpp`, `src/Deck.h/.cpp`
`Card` holds a rank (2–Ace) and suit (Heart, Spade, Club, Diamond) and knows its chip value. `Deck` contains 52 cards and handles shuffling, drawing, and resetting. The deck resets each round so played and discarded cards never reappear.

### `ScoreContext` — `src/ScoreContext.h`
A lightweight data container holding `chips` and `mult`. `finalScore()` returns `chips × mult`. This is the object that modifiers operate on — each modifier receives a reference to `ScoreContext` and modifies it before the final score is computed.

### `HandEvaluator` — `src/HandEvaluator.h/.cpp`
Detects the poker hand type from a set of cards and returns base chips, base mult, and which cards contributed to the hand (scoring cards). Hand definitions are stored in the static `handDefinitions` table — a single source of truth used by both the scoring system and the in-game hand reference display.

### `ScoringSystem` — `src/ScoringSystem.h/.cpp`
Calls `HandEvaluator::evaluate()`, adds scoring card chip values to base chips, then applies each active modifier in order via `IModifier::apply(ScoreContext&)`. The final score is `ScoreContext::finalScore()`.

### `IModifier` — `src/modifiers/IModifier.h`
Abstract interface that all modifiers implement. Defines four pure virtual methods: `apply(ScoreContext&)`, `getName()`, `getDescription()`, `getCost()`.

### Concrete Modifiers — `src/modifiers/`
Twelve modifiers, each in its own `.h/.cpp` pair:

| Modifier | Effect | Cost |
|---|---|---|
| Chip In | +25 chips | 5 |
| Small Bonus | +3 mult | 6 |
| Chipper Reward | +20 to +80 chips (random) | 6 |
| Pocket Change | +50 chips | 7 |
| Fortune's Favor | +2 to +16 mult (random) | 7 |
| Lucky Seven | +7 mult | 8 |
| Double Trouble | Mult × 2 | 10 |
| Mega Payout | +90 chips | 10 |
| Midas Touch | +12 mult | 14 |
| Triple Threat | Mult × 3 | 16 |
| Fourfold Fortune | Mult × 4 | 20 |

### `ModifierFactory` — `src/ModifierFactory.h/.cpp`
Creates modifier objects by string type identifier. The sole place where `new` is called on concrete modifiers. `ShopSystem` calls `ModifierFactory::createModifier()` when the player buys an item.

### `ShopSystem` — `src/ShopSystem.h/.cpp`
Manages the between-round shop. Randomly selects items from the full modifier pool each round (configurable via `SHOP_SIZE`). Handles buying, selling, rerolling, and modifier management (swap positions). Enforces the modifier slot limit (`MODIFIER_LIMIT = 5`).

### `RunSession` — `src/RunSession.h/.cpp`
The master game controller. Owns the game loop, deck, scoring system, shop system, and active modifier list. Coordinates all phases: playing hands, calculating scores, opening the shop, and scaling difficulty each round. Also handles hand sorting and the hand reference display.

---

## Pattern Usage

### Factory Pattern (Creational)
**Location:** `ModifierFactory::createModifier()` in `src/ModifierFactory.cpp`

All modifier objects are created through `ModifierFactory`. No other class calls `new` on a concrete modifier type. This centralizes object creation and decouples the rest of the system from concrete modifier types.

### Decorator-like Modifier Chain (Structural)
**Location:** `ScoringSystem::calculate()` in `src/ScoringSystem.cpp`

`ScoreContext` is the object being "decorated." Each `IModifier::apply(ScoreContext&)` call adds a layer of behavior — modifying chips or mult — before the final score is computed. The order of modifiers in `activeModifiers` matters significantly, just as decorator wrapping order matters.

### Strategy Pattern (Behavioral)
**Location:** `HandEvaluator::evaluate()` in `src/HandEvaluator.cpp`

Hand detection and base score assignment is isolated in `HandEvaluator`, separate from modifier application in `ScoringSystem`. The scoring rule can be swapped without touching modifier logic or the game loop.

### Facade (Structural — informal)
**Location:** `RunSession` in `src/RunSession.h/.cpp`

`RunSession` acts as a Facade, coordinating `Deck`, `ScoringSystem`, `ShopSystem`, and the modifier list behind a single `startRun()` interface. `main.cpp` only needs to create a `RunSession` and call `startRun()`.

---

## Modification Log — Adding SquareUp

`SquareScoreModifier` (`src/modifiers/SquareScoreModifier.h/.cpp`) can be added to demonstrate the extensibility of the Factory and IModifier design.

**Files created:**
- `src/modifiers/SquareScoreModifier.h` — declares class inheriting `IModifier`
- `src/modifiers/SquareScoreModifier.cpp` — implements `apply()`, `getName()`, `getDescription()`, `getCost()`

**Files modified:**
- `src/ModifierFactory.cpp` — added `#include "modifiers/SquareScoreModifier.h"` and one `if` branch in `createModifier()`
- `src/ShopSystem.cpp` — added one entry in `buildItemPool()`

No other files were changed. `RunSession`, `ScoringSystem`, `IModifier`, and all other modifiers required zero modification — demonstrating that the system is open for extension and closed for modification (Open/Closed Principle).

---

## AI Usage Disclosure

This project was built with significant assistance from Claude (Anthropic) via claude.ai.

**What AI helped with:**
- Overall architecture design and class relationship planning
- Implementing all C++ source files (`.h` and `.cpp`)
- Debugging runtime issues (hand detection bugs, cin buffer issues, deck reset bugs)
- Game design decisions (modifier balance, scoring formula, economy tuning)
- Writing this analysis and README

**What the student contributed:**
- All final design decisions (theme, modifier names, economy parameters, balance choices)
- Testing and identifying bugs during gameplay
- Directing the development process and feature requirements
- Understanding and being able to explain all code written

The student understands the codebase and can explain every class, function, and design pattern decision referenced in this document.
