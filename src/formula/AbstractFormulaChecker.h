#ifndef STORM_FORMULA_ABSTRACTFORMULACHECKER_H_
#define STORM_FORMULA_ABSTRACTFORMULACHECKER_H_

namespace storm {
namespace property {

template <class T> class AbstractFormulaChecker;

} //namespace property
} //namespace storm


#include "src/formula/AbstractFormula.h"
#include <memory>

namespace storm {
namespace property {

/*!
 *	@brief	Base class for all formula checkers.
 *
 *	A formula checker is used to check if a given formula is valid in some
 *	logic. Hence, this pure virtual base class should be subclassed for
 *	every logic we support.
 *
 *	Every subclass must implement validate(). It gets a pointer to an
 *	AbstractFormula object and should return if the subtree represented by
 *	this formula is valid in the logic.
 *
 *	Usually, this will be implemented like this:
 *	@code
 *	if	(
 *			dynamic_cast<const And<T>*>(formula) ||
 *			dynamic_cast<const Not<T>*>(formula) ||
 *			dynamic_cast<const Or<T>*>(formula)
 *		) {
 *		return formula->validate(*this);
 *	} else	return false;
 *	@endcode
 *
 *	Every formula class implements a validate() method itself which calls
 *	validate() on the given checker for every child in the formula tree.
 *
 *	If the formula structure is not an actual tree, but an directed acyclic
 *	graph, the shared subtrees will be checked twice. If we have directed
 *	cycles, we will have infinite recursions.
 */
template <class T>
class AbstractFormulaChecker {
	public:
		/*!
		 * Virtual destructor
		 * To ensure that the right destructor is called
		 */
		virtual ~AbstractFormulaChecker() {
			//intentionally left empty
		}

		/*!
		 *	@brief Checks if the given formula is valid in some logic.
		 *
		 *	Every subclass must implement this method and check, if the
		 *	formula object is valid in the logic of the subclass.
		 *
		 *	@param formula A pointer to some formula object.
		 *	@return true iff the formula is valid.
		 */
		virtual bool validate(std::shared_ptr<AbstractFormula<T>> const & formula) const = 0;
};

} // namespace property
} // namespace storm

#endif
