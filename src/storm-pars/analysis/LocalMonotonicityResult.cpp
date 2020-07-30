#include "LocalMonotonicityResult.h"

namespace storm {
    namespace analysis {

        template <typename VariableType>
        LocalMonotonicityResult<VariableType>::LocalMonotonicityResult(uint_fast64_t numberOfStates) {
            stateMonRes = std::vector<std::shared_ptr<MonotonicityResult<VariableType>>>(numberOfStates, nullptr);
            globalMonotonicityResult = std::make_shared<MonotonicityResult<VariableType>>();
            statesMonotone = storm::storage::BitVector(numberOfStates, false);
        }

        template <typename VariableType>
        typename LocalMonotonicityResult<VariableType>::Monotonicity LocalMonotonicityResult<VariableType>::getMonotonicity(uint_fast64_t state, VariableType var) const {
            if (stateMonRes[state] != nullptr) {
                return stateMonRes[state]->getMonotonicity(var);
            } else if (statesMonotone[state]) {
                return Monotonicity::Constant;
            } else{
                return Monotonicity::Unknown;
            }
        }

        template <typename VariableType>
        std::shared_ptr<MonotonicityResult<VariableType>> LocalMonotonicityResult<VariableType>::getGlobalMonotonicityResult() const {
            return globalMonotonicityResult;
        }

        template <typename VariableType>
        void LocalMonotonicityResult<VariableType>::setMonotonicity(uint_fast64_t state, VariableType var, typename LocalMonotonicityResult<VariableType>::Monotonicity mon) {
            if (stateMonRes[state] == nullptr) {
                stateMonRes[state] = std::make_shared<MonotonicityResult<VariableType>>();
            }
            stateMonRes[state]->addMonotonicityResult(var, mon);
            globalMonotonicityResult->updateMonotonicityResult(var, mon);
            if (mon == Monotonicity::Unknown || mon == Monotonicity::Not) {
                statesMonotone.set(state, false);
            } else {
                statesMonotone.set(state, stateMonRes[state]->isAllMonotonicity());
                if (isDone()) {
                    globalMonotonicityResult->setDone();
                }
            }
        }

        template <typename VariableType>
        std::shared_ptr<LocalMonotonicityResult<VariableType>> LocalMonotonicityResult<VariableType>::copy() {
            std::shared_ptr<LocalMonotonicityResult<VariableType>> copy = std::make_shared<LocalMonotonicityResult<VariableType>>(stateMonRes.size());
            for (auto state = 0; state < stateMonRes.size(); state++) {
                if (stateMonRes[state] != nullptr) {
                    copy->setMonotonicityResult(state, stateMonRes[state]->copy());
                }
            }
            copy->setGlobalMonotonicityResult(this->getGlobalMonotonicityResult()->copy());
            copy->setStatesMonotone(statesMonotone);
            return copy;
        }

        template <typename VariableType>
        bool LocalMonotonicityResult<VariableType>::isDone() const {
            return done || statesMonotone.full();
        }

        template <typename VariableType>
        void LocalMonotonicityResult<VariableType>::setDone(bool done) {
            this->done = done;
        }

        template <typename VariableType>
        void LocalMonotonicityResult<VariableType>::setIndexMinimize(int i) {
            assert (indexMinimize == -1);
            this->indexMinimize = i;
        }

        template <typename VariableType>
        void LocalMonotonicityResult<VariableType>::setIndexMaximize(int i) {
            this->indexMaximize = i;
        }

        template <typename VariableType>
        int LocalMonotonicityResult<VariableType>::getIndexMinimize() const {
            return indexMinimize;
        }

        template <typename VariableType>
        int LocalMonotonicityResult<VariableType>::getIndexMaximize() const {
            return indexMaximize;
        }

        template <typename VariableType>
        bool LocalMonotonicityResult<VariableType>::isNoMonotonicity() const {
            return statesMonotone.empty();
        }

        template <typename VariableType>
        void LocalMonotonicityResult<VariableType>::setMonotonicityResult(uint_fast64_t state, std::shared_ptr<MonotonicityResult<VariableType>> monRes) {
            this->stateMonRes[state] = monRes;
        }

        template <typename VariableType>
        void LocalMonotonicityResult<VariableType>::setGlobalMonotonicityResult(std::shared_ptr<MonotonicityResult<VariableType>> monRes) {
            this->globalMonotonicityResult = monRes;
        }

        template <typename VariableType>
        void LocalMonotonicityResult<VariableType>::setStatesMonotone(storm::storage::BitVector statesMonotone) {
            this->statesMonotone = statesMonotone;
        }

        template <typename VariableType>
        void LocalMonotonicityResult<VariableType>::setConstant(uint_fast64_t state) {
            this->statesMonotone.set(state);
        }

        template <typename VariableType>
        std::string LocalMonotonicityResult<VariableType>::toString() const {
            std::string result = "Local Monotonicity Result: \n";
            for (auto i = 0; i < stateMonRes.size(); ++i) {
                result += "state ";
                result += std::to_string(i);
                if (stateMonRes[i] != nullptr) {
                    result += stateMonRes[i]->toString();
                } else if (statesMonotone[i]) {
                    result += "constant";
                } else {
                    result += "not analyzed";
                }
                result += "\n";
            }
            return result;
        }

        template class LocalMonotonicityResult<storm::RationalFunctionVariable>;
    }
}
