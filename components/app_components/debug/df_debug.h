#pragma once
#ifndef DATAFLOW_COMPONENTS_DF_DEBUG_H_INCLUDED
#define DATAFLOW_COMPONENTS_DF_DEBUG_H_INCLUDED

// Standard includes
#include <string>

// Project includes
#include "dataflow.h"


/**
 * This class implements a generic debugging facility which prints
 * the messages it receives to the standard output.
 *
 * Ports:
 *
 * [input] "in"   - Used to receive messages which are then printed to the
 *                  standard output.
 *
 * [output] "out" - When this port is connected the received messages are forwarded
 *                  to this port after being printed. Otherwise the messages are
 *                  dropped.
 */
class DF_Debug : public Component {
public:

	DF_Debug();

	virtual void process() override;
};


#endif // DATAFLOW_COMPONENTS_DF_DEBUG_H_INCLUDED
