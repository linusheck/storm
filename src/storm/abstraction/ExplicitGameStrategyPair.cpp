#include "storm/abstraction/ExplicitGameStrategyPair.h"

namespace storm {
namespace abstraction {

ExplicitGameStrategyPair::ExplicitGameStrategyPair(uint64_t numberOfPlayer1States, uint64_t numberOfPlayer2States)
    : player1Strategy(numberOfPlayer1States), player2Strategy(numberOfPlayer2States) {
    // Intentionally left empty.
}

ExplicitGameStrategyPair::ExplicitGameStrategyPair(ExplicitGameStrategy&& player1Strategy, ExplicitGameStrategy&& player2Strategy)
    : player1Strategy(std::move(player1Strategy)), player2Strategy(std::move(player2Strategy)) {
    // Intentionally left empty.
}

ExplicitGameStrategy& ExplicitGameStrategyPair::getPlayer1Strategy() {
    return player1Strategy;
}

ExplicitGameStrategy const& ExplicitGameStrategyPair::getPlayer1Strategy() const {
    return player1Strategy;
}

ExplicitGameStrategy& ExplicitGameStrategyPair::getPlayer2Strategy() {
    return player2Strategy;
}

ExplicitGameStrategy const& ExplicitGameStrategyPair::getPlayer2Strategy() const {
    return player2Strategy;
}

uint64_t ExplicitGameStrategyPair::getNumberOfUndefinedPlayer1States() const {
    return player1Strategy.getNumberOfUndefinedStates();
}

uint64_t ExplicitGameStrategyPair::getNumberOfUndefinedPlayer2States() const {
    return player2Strategy.getNumberOfUndefinedStates();
}

std::ostream& operator<<(std::ostream& out, ExplicitGameStrategyPair const& strategyPair) {
    out << "player 1 strategy: " << std::endl << strategyPair.getPlayer1Strategy() << std::endl;
    out << "player 2 strategy: " << std::endl << strategyPair.getPlayer2Strategy() << std::endl;
    return out;
}

}  // namespace abstraction
}  // namespace storm
