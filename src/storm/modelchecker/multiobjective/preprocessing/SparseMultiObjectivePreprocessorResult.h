#pragma once

#include <boost/optional.hpp>
#include <memory>
#include <vector>

#include "storm/logic/Formulas.h"
#include "storm/modelchecker/multiobjective/Objective.h"
#include "storm/storage/BitVector.h"

#include "storm/exceptions/UnexpectedException.h"

namespace storm {
namespace modelchecker {
namespace multiobjective {
namespace preprocessing {

template<class SparseModelType>
struct SparseMultiObjectivePreprocessorResult {
    enum class QueryType { Achievability, Quantitative, Pareto };

    // Original data
    storm::logic::MultiObjectiveFormula const& originalFormula;
    SparseModelType const& originalModel;

    // The preprocessed model and objectives
    std::shared_ptr<SparseModelType> preprocessedModel;
    std::vector<Objective<typename SparseModelType::ValueType>> objectives;

    // Data about the query
    QueryType queryType;

    // Indices of the objectives that can potentially yield infinite reward
    storm::storage::BitVector maybeInfiniteRewardObjectives;

    SparseMultiObjectivePreprocessorResult(storm::logic::MultiObjectiveFormula const& originalFormula, SparseModelType const& originalModel)
        : originalFormula(originalFormula), originalModel(originalModel) {
        // Intentionally left empty
    }

    uint64_t getNumberOfTotalRewardFormulas() const {
        uint64_t count = 0;
        for (auto const& obj : objectives) {
            if (obj.formula->isRewardOperatorFormula() && obj.formula->getSubformula().isTotalRewardFormula()) {
                ++count;
            }
        }
        return count;
    }

    bool containsOnlyTotalRewardFormulas() const {
        return getNumberOfTotalRewardFormulas() == objectives.size();
    }

    uint64_t getNumberOfLongRunAverageRewardFormulas() const {
        uint64_t count = 0;
        for (auto const& obj : objectives) {
            if (obj.formula->isRewardOperatorFormula() && obj.formula->getSubformula().isLongRunAverageRewardFormula()) {
                ++count;
            }
        }
        return count;
    }

    bool containsLongRunAverageRewardFormulas() const {
        return getNumberOfLongRunAverageRewardFormulas() > 0;
    }

    bool containsOnlyTrivialObjectives() const {
        // Trivial objectives are either total reward formulas, LRA reward formulas or single-dimensional step or time bounded cumulative reward formulas
        for (auto const& obj : objectives) {
            if (obj.formula->isRewardOperatorFormula() && obj.formula->getSubformula().isTotalRewardFormula()) {
                continue;
            }
            if (obj.formula->isRewardOperatorFormula() && obj.formula->getSubformula().isLongRunAverageRewardFormula()) {
                continue;
            }
            if (obj.formula->isRewardOperatorFormula() && obj.formula->getSubformula().isCumulativeRewardFormula()) {
                auto const& subf = obj.formula->getSubformula().asCumulativeRewardFormula();
                if (!subf.isMultiDimensional() && (subf.getTimeBoundReference().isTimeBound() || subf.getTimeBoundReference().isStepBound())) {
                    continue;
                }
            }
            // Reaching this point means that the objective is considered as non-trivial
            return false;
        }
        return true;
    }

    void printToStream(std::ostream& out) const {
        out << std::endl;
        out << "---------------------------------------------------------------------------------------------------------------------------------------"
            << std::endl;
        out << "                                                       Multi-objective Query                                              " << std::endl;
        out << "---------------------------------------------------------------------------------------------------------------------------------------"
            << std::endl;
        out << std::endl;
        out << "Original Formula: " << std::endl;
        out << "--------------------------------------------------------------" << std::endl;
        out << "\t" << originalFormula << std::endl;
        out << std::endl;
        out << "The query considers " << objectives.size() << " objectives:" << std::endl;
        out << "--------------------------------------------------------------" << std::endl;
        for (auto const& obj : objectives) {
            obj.printToStream(out);
            out << std::endl;
        }
        out << "Number of Long-Run-Average Reward Objectives (after preprocessing): " << getNumberOfLongRunAverageRewardFormulas() << "." << std::endl;
        out << "Number of Total Reward Objectives (after preprocessing): " << getNumberOfTotalRewardFormulas() << "." << std::endl;
        out << "--------------------------------------------------------------" << std::endl;
        out << std::endl;
        out << "Original Model Information:" << std::endl;
        originalModel.printModelInformationToStream(out);
        out << std::endl;
        out << "Preprocessed Model Information:" << std::endl;
        preprocessedModel->printModelInformationToStream(out);
        out << std::endl;
        out << "---------------------------------------------------------------------------------------------------------------------------------------"
            << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& out, SparseMultiObjectivePreprocessorResult<SparseModelType> const& ret) {
        ret.printToStream(out);
        return out;
    }
};
}  // namespace preprocessing
}  // namespace multiobjective
}  // namespace modelchecker
}  // namespace storm
