#include "df_function.h"

void DF_Function::process()
{
	// Calling user provided function
	m_function(m_ports);
}
