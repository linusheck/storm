#ifndef STORM_LATTICEEXTENDER_H
#define STORM_LATTICEEXTENDER_H

#include "storm/api/storm.h"
#include "storm/logic/Formula.h"
#include "storm/models/sparse/Model.h"
#include "storm/storage/expressions/BinaryRelationExpression.h"
#include "storm/storage/expressions/VariableExpression.h"

#include "storm-pars/analysis/Order.h"
#include "storm-pars/analysis/MonotonicityResult.h"
#include "storm-pars/storage/ParameterRegion.h"


namespace storm {
    namespace analysis {
        template<typename ValueType, typename ConstantType>
        class OrderExtender {

        public:
            typedef typename utility::parametric::CoefficientType<ValueType>::type CoefficientType;
            typedef typename utility::parametric::VariableType<ValueType>::type VariableType;

           /*!
            * Constructs OrderExtender which can extend an order
            *
            * @param model The model for which the order should be extended.
            */
           OrderExtender(std::shared_ptr<models::sparse::Model<ValueType>> model, std::shared_ptr<logic::Formula const> formula,  storage::ParameterRegion<ValueType> region);

           /*!
            * Creates an order based on the given formula.
            *
            * @return A triple with a pointer to the order and two states of which the current place in the order
            *         is unknown but needed. When the states have as number the number of states, no states are
            *         unplaced but needed.
            */
            std::tuple<Order*, uint_fast64_t, uint_fast64_t> toOrder(std::shared_ptr<MonotonicityResult<VariableType>> monRes);

            /*!
             * Creates an order based on the given min/max values
             * Might be unsufficient, as no assumptions will be made
             *
             * @param minValues The lower bound of reaching target state
             * @param maxValues The upper bound of reaching target state
             * @return possibly unsufficient order
             */
            Order* toOrder(std::vector<ConstantType> minValues, std::vector<ConstantType> maxValues, std::shared_ptr<MonotonicityResult<VariableType>> monRes);

            /*!
             * Extends the order based on the given assumption.
             *
             * @param order The order.
             * @param assumption The assumption on states.
             * @return A triple with a pointer to the order and two states of which the current place in the order
             *         is unknown but needed. When the states have as number the number of states, no states are
             *         unplaced but needed.
             */
            std::tuple<Order*, uint_fast64_t, uint_fast64_t> extendOrder(Order* order, std::shared_ptr<MonotonicityResult<VariableType>> monRes, std::shared_ptr<expressions::BinaryRelationExpression> assumption = nullptr);

        private:
            void init(std::shared_ptr<models::sparse::Model<ValueType>> model);

            Order* getBottomTopOrder();

            Order::NodeComparison addStatesBasedOnMinMax(Order* order, uint_fast64_t state1, uint_fast64_t state2);

            std::tuple<Order*, uint_fast64_t, uint_fast64_t> extendOrder(Order* order, bool useAssumptions, std::shared_ptr<MonotonicityResult<VariableType>> monRes);

            std::pair<uint_fast64_t, uint_fast64_t> extendByForwardReasoning(Order* order, uint_fast64_t currentState, storage::BitVector* successors);

            std::pair<uint_fast64_t, uint_fast64_t> extendByBackwardReasoning(Order* order, uint_fast64_t currentState, storage::BitVector* successors);

            Order* bottomTopOrder = nullptr;

            void handleAssumption(Order* order, std::shared_ptr<expressions::BinaryRelationExpression> assumption);

            void getMinMaxValues();

            std::vector<ConstantType> minValues;

            std::vector<ConstantType> maxValues;

            storage::SparseMatrix<ValueType> matrix;

            std::shared_ptr<models::sparse::Model<ValueType>> model;

            std::map<uint_fast64_t, storage::BitVector*> stateMap;

            bool usePLA;

            bool cyclic;

            std::shared_ptr<logic::Formula const> formula;

            storage::ParameterRegion<ValueType> region;

            uint_fast64_t numberOfStates;

            ValueType getMatrixEntry(uint_fast64_t rowIndex, uint_fast64_t columnIndex);

            typename MonotonicityResult<typename OrderExtender<ValueType, ConstantType>::VariableType>::Monotonicity checkTransitionMonRes(uint_fast64_t from, uint_fast64_t to, typename OrderExtender<ValueType, ConstantType>::VariableType param);

            typename MonotonicityResult<typename OrderExtender<ValueType, ConstantType>::VariableType>::Monotonicity checkTransitionMonRes(ValueType function, typename OrderExtender<ValueType, ConstantType>::VariableType param);

            void checkParOnStateMonRes(uint_fast64_t s, const std::vector<uint_fast64_t>& succ, typename OrderExtender<ValueType, ConstantType>::VariableType param, std::shared_ptr<MonotonicityResult<VariableType>> monResult);

            std::set<VariableType> params;

            ValueType getDerivative(ValueType function, VariableType param);

            std::unordered_map<ValueType, std::unordered_map<VariableType, ValueType>> derivatives;
        };
    }
}

#endif //STORM_ORDEREXTENDER_H
