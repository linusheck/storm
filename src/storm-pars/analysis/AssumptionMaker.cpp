#include "AssumptionMaker.h"

namespace storm {
    namespace analysis {
        template<typename ValueType, typename ConstantType>
        AssumptionMaker<ValueType, ConstantType>::AssumptionMaker(storage::SparseMatrix<ValueType> matrix) : assumptionChecker(matrix){
            numberOfStates = matrix.getColumnCount();
            expressionManager = std::make_shared<expressions::ExpressionManager>(expressions::ExpressionManager());
            for (uint_fast64_t i = 0; i < this->numberOfStates; ++i) {
                expressionManager->declareRationalVariable(std::to_string(i));
            }
        }

        template <typename ValueType, typename ConstantType>
        std::map<std::shared_ptr<expressions::BinaryRelationExpression>, AssumptionStatus> AssumptionMaker<ValueType, ConstantType>::createAndCheckAssumptions(uint_fast64_t val1, uint_fast64_t val2, std::shared_ptr<Order> order, storage::ParameterRegion<ValueType> region) const {
            std::map<std::shared_ptr<expressions::BinaryRelationExpression>, AssumptionStatus> result;
            expressions::Variable var1 = expressionManager->getVariable(std::to_string(val1));
            expressions::Variable var2 = expressionManager->getVariable(std::to_string(val2));
            auto assumption = createAndCheckAssumption(var1, var2, expressions::BinaryRelationExpression::RelationType::Greater, order, region);
            if (assumption.second != AssumptionStatus::INVALID) {
                result.insert(assumption);
                if (assumption.second == AssumptionStatus::VALID) {
                    assert (createAndCheckAssumption(var2, var1, expressions::BinaryRelationExpression::RelationType::Greater, order, region).second != AssumptionStatus::VALID
                            && createAndCheckAssumption(var1, var2, expressions::BinaryRelationExpression::RelationType::Equal, order, region).second != AssumptionStatus::VALID);
                    return result;
                }
            }
            assumption = createAndCheckAssumption(var2, var1, expressions::BinaryRelationExpression::RelationType::Greater, order, region);
            if (assumption.second != AssumptionStatus::INVALID) {
                if (assumption.second == AssumptionStatus::VALID) {
                    result.clear();
                    result.insert(assumption);
                    assert (createAndCheckAssumption(var1, var2, expressions::BinaryRelationExpression::RelationType::Equal, order, region).second != AssumptionStatus::VALID);
                    return result;
                }
                result.insert(assumption);
            }
            assumption = createAndCheckAssumption(var1, var2, expressions::BinaryRelationExpression::RelationType::Equal, order, region);
            if (assumption.second != AssumptionStatus::INVALID) {
                if (assumption.second == AssumptionStatus::VALID) {
                    result.clear();
                    result.insert(assumption);
                    return result;
                }
                result.insert(assumption);
            }
            return result;
        }

        template <typename ValueType, typename ConstantType>
        void AssumptionMaker<ValueType, ConstantType>::initializeCheckingOnSamples(std::shared_ptr<logic::Formula const> formula, std::shared_ptr<models::sparse::Dtmc<ValueType>> model, storage::ParameterRegion<ValueType> region, uint_fast64_t numberOfSamples){
            assumptionChecker.initializeCheckingOnSamples(formula, model, region, numberOfSamples);
        }

        template <typename ValueType, typename ConstantType>
        void AssumptionMaker<ValueType, ConstantType>::setSampleValues(std::vector<std::vector<ConstantType>>const & samples) {
            assumptionChecker.setSampleValues(samples);
        }

        template <typename ValueType, typename ConstantType>
        std::pair<std::shared_ptr<expressions::BinaryRelationExpression>, AssumptionStatus> AssumptionMaker<ValueType, ConstantType>::createAndCheckAssumption(expressions::Variable var1, expressions::Variable var2, expressions::BinaryRelationExpression::RelationType relationType, std::shared_ptr<Order> order, storage::ParameterRegion<ValueType> region) const {
            auto assumption = std::make_shared<expressions::BinaryRelationExpression>(expressions::BinaryRelationExpression(*expressionManager, expressionManager->getBooleanType(), var2.getExpression().getBaseExpressionPointer(), var1.getExpression().getBaseExpressionPointer(), relationType));
            AssumptionStatus validationResult = assumptionChecker.validateAssumption(assumption, order, region);
            return std::pair<std::shared_ptr<expressions::BinaryRelationExpression>, AssumptionStatus>(assumption, validationResult);
        }

        template class AssumptionMaker<RationalFunction, double>;
        template class AssumptionMaker<RationalFunction, RationalNumber>;
    }
}
