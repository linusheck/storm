#include "MonotonicityChecker.h"

namespace storm {
    namespace analysis {
        /*** Constructor ***/
        template <typename ValueType>
        MonotonicityChecker<ValueType>::MonotonicityChecker(storage::SparseMatrix<ValueType> matrix) {
            this->matrix = matrix;
        }

        /*** Public methods ***/
        template <typename ValueType>
        typename MonotonicityChecker<ValueType>::Monotonicity MonotonicityChecker<ValueType>::checkLocalMonotonicity(std::shared_ptr<Order> const& order, uint_fast64_t state, VariableType const& var, storage::ParameterRegion<ValueType> const& region) {
            // Create + fill Vector containing the Monotonicity of the transitions to the succs
            auto row = matrix.getRow(state);
            // Ignore if all entries are constant
            bool ignore = true;

            std::vector<uint_fast64_t> succs;
            std::vector<Monotonicity> succsMonUnsorted;
            for (auto entry : row) {
                auto succState = entry.getColumn();
                succsMonUnsorted.push_back(checkTransitionMonRes(entry.getValue(), var, region));
                succs.push_back(succState);
                ignore &= entry.getValue().isConstant();
            }
            if (ignore) {
                return Monotonicity::Constant;
            }
            auto succsSorted = order->sortStates(&succs);

            uint_fast64_t succSize = succs.size();
            if (succsSorted[succSize - 1] == matrix.getColumnCount()) {
                return Monotonicity::Unknown;
            }

            if (succSize == 2) {
                // In this case we can ignore the last entry, as this will have a probability of 1 - the other
                succSize = 1;
            }

            // First check as long as it stays constant and either incr or decr
            bool allowedToSwap = true;
            Monotonicity localMonotonicity = Monotonicity::Constant;
            uint_fast64_t index = 0;
            while (index < succSize && localMonotonicity == Monotonicity::Constant) {
                auto itr = std::find(succs.begin(), succs.end(), succsSorted[index]);
                auto newIndex = std::distance(succs.begin(), itr);
                auto transitionMon = succsMonUnsorted[newIndex];
                localMonotonicity = transitionMon;
                if (transitionMon == Monotonicity::Not && succSize != 1) {
                    localMonotonicity = Monotonicity::Unknown;
                }
                index++;
            }

            while (index < succSize && localMonotonicity != Monotonicity::Not && localMonotonicity != Monotonicity::Unknown) {
                // We get here as soon as we have seen incr/decr once
                auto itr = std::find(succs.begin(), succs.end(), succsSorted[index]);
                auto newIndex = std::distance(succs.begin(), itr);
                auto transitionMon = succsMonUnsorted[newIndex];

                if (transitionMon == Monotonicity::Not || transitionMon == Monotonicity::Unknown) {
                    return Monotonicity::Unknown;
                }
                if (allowedToSwap) {
                    // So far we have only seen constant and either incr or decr, but not both
                    if (transitionMon != Monotonicity::Constant && transitionMon != localMonotonicity) {
                        allowedToSwap = false;
                    }
                } else if (!allowedToSwap) {
                    // So we have been at the point where we changed from incr to decr (or decr to incr)
                    if (transitionMon == localMonotonicity || transitionMon == Monotonicity::Not || transitionMon == Monotonicity::Unknown) {
                        localMonotonicity = Monotonicity::Unknown;
                    }
                }
                index++;
            }
            return localMonotonicity;
        }

        /*** Private methods ***/
        template <typename ValueType>
        typename MonotonicityChecker<ValueType>::Monotonicity MonotonicityChecker<ValueType>::checkTransitionMonRes(ValueType function, typename MonotonicityChecker<ValueType>::VariableType param, typename MonotonicityChecker<ValueType>::Region region) {
            std::pair<bool, bool> res = MonotonicityChecker<ValueType>::checkDerivative(getDerivative(function, param), region);
            if (res.first && !res.second) {
                return Monotonicity::Incr;
            } else if (!res.first && res.second) {
                return Monotonicity::Decr;
            } else if (res.first && res.second) {
                return Monotonicity::Constant;
            } else {
                return Monotonicity::Not;
            }
        }

        template <typename ValueType>
        ValueType MonotonicityChecker<ValueType>::getDerivative(ValueType function, typename MonotonicityChecker<ValueType>::VariableType var) {
            if (function.isConstant()) {
                return utility::zero<ValueType>();
            }
            if ((derivatives[function]).find(var) == (derivatives[function]).end()) {
                (derivatives[function])[var] = function.derivative(var);
            }
            return (derivatives[function])[var];
        }

        template class MonotonicityChecker<RationalFunction>;
    }
}
