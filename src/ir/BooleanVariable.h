/*
 * BooleanVariable.h
 *
 *  Created on: 08.01.2013
 *      Author: chris
 */

#ifndef BOOLEANVARIABLE_H_
#define BOOLEANVARIABLE_H_

namespace storm {

namespace ir {

class BooleanVariable : public Variable {
public:
	BooleanVariable() {

	}

	BooleanVariable(std::string variableName) : Variable(variableName) {

	}

	virtual ~BooleanVariable() {

	}

	virtual std::string toString() {
		return variableName + ": bool;";
	}
};

}

}

BOOST_FUSION_ADAPT_STRUCT(
    storm::ir::BooleanVariable,
    (std::string, variableName)
)

#endif /* BOOLEANVARIABLE_H_ */
