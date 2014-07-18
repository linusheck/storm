/*
 * AbstractRewardPathFormula.h
 *
 *  Created on: May 15, 2014
 *      Author: Manuel S. Weiand
 */

#ifndef STORM_FORMULA_PRCTL_ABSTRACTREWARDPATHFORMULA_H_
#define STORM_FORMULA_PRCTL_ABSTRACTREWARDPATHFORMULA_H_

#include "src/formula/prctl/AbstractPrctlFormula.h"

namespace storm {
namespace property {
namespace prctl {

/*! Base class for reward path formulas.
 *
 *  Reward path formulas are subformulas of reward bound operators.
 *  They may not be subformulas of a probabilitic bound operator.
 *
 */
template <class T>
class AbstractRewardPathFormula : public virtual storm::property::prctl::AbstractPrctlFormula<T> {

public:
	/*!
	 * Empty virtual destructor.
	 */
	virtual ~AbstractRewardPathFormula() {
		// Intentionally left empty
	}

	/*!
	 * Clones the called object.
	 *
	 * Performs a "deep copy", i.e. the subtrees of the new object are clones of the original ones
	 *
	 * @note This function is not implemented in this class.
	 * @returns a new AND-object that is identical the called object.
	 */
	virtual std::shared_ptr<AbstractRewardPathFormula<T>> clone() const = 0;

	/*!
	 * Calls the model checker to check this formula.
	 * Needed to infer the correct type of formula class.
	 *
	 * @note This function should only be called in a generic check function of a model checker class. For other uses,
	 *       the methods of the model checker should be used.
	 *
	 * @note This function is not implemented in this class.
	 *
	 * @returns A vector indicating the probability that the formula holds for each state.
	 */
	virtual std::vector<T> check(const storm::modelchecker::prctl::AbstractModelChecker<T>& modelChecker, bool qualitative) const = 0;
};

} //namespace prctl
} //namespace property
} //namespace storm



#endif /* STORM_FORMULA_PRCTL_ABSTRACTREWARDPATHFORMULA_H_ */
