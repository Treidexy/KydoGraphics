#pragma once

#include "Kydo/Core.h"

#include <CL/cl2.hpp>

namespace Kydo
{
	class KYDO_API Context;

	class KYDO_API Program
	{
	private:
		Context *context;
		cl::Program prog;
		cl::Kernel kernel;
	public:
		Program(Context *context, const cl::string &src, const char *entry);
		
		cl::Kernel &GetKernel();
	};
}

#include "Kydo/Context.h"