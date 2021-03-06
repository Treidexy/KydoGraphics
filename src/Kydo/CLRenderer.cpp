#include "Kydo/CLRenderer.h"

#include <algorithm>

namespace Kydo
{
	inline const char *clGetErrorString(cl_int ec);
	inline bool CheckEc(cl_int ec, bool &alive, int ln)
	{
		if (ec)
		{
			std::printf("OpenCL Error at Line %i: %s\n", ln, clGetErrorString(ec));
			alive = false;
			return true;
		}

		return false;
	}

	#define CHECK_EC(ec) if (CheckEc(ec, this->alive, __LINE__)) return

	CLRenderer::CLRenderer(Window &win, std::string_view source)
		: wnd(&win), src(source)
	{
		cl_int ec;
		ec = cl::Platform::get(&platform); CHECK_EC(ec);
		std::vector<cl::Device> devices;
		ec = platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
		if (ec == CL_DEVICE_NOT_FOUND || ec == CL_DEVICE_NOT_AVAILABLE || devices.empty())
		{
			alive = false;
			return;
		}
		CHECK_EC(ec);
		device = devices.front();

		std::printf("Using OpenCL Device '%s'\n", device.getInfo<CL_DEVICE_NAME>(&ec).c_str()); CHECK_EC(ec);

		context = cl::Context(device, NULL, NULL, NULL, &ec); CHECK_EC(ec);
		q = cl::CommandQueue(context, device, cl::QueueProperties::None, &ec); CHECK_EC(ec);

		prog = cl::Program(context, src, false, &ec); CHECK_EC(ec);
		ec = prog.build(device);
		if (ec == CL_BUILD_PROGRAM_FAILURE)
		{
			alive = false;
			auto log = prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(&ec); CHECK_EC(ec);
			for (auto msg : log)
				puts(msg.second.c_str());
			return;
		}
		CHECK_EC(ec);

		pixelMem = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, wnd->PixelCount * sizeof(COLORREF), wnd->Pixels, &ec); CHECK_EC(ec);
		clearKernel = cl::Kernel(prog, "Clear", &ec); CHECK_EC(ec);
		ec = clearKernel.setArg(0, pixelMem); CHECK_EC(ec);
		drawKernel = cl::Kernel(prog, "Draw", &ec); CHECK_EC(ec);
		ec = drawKernel.setArg(0, pixelMem); CHECK_EC(ec);
	}

	CLRenderer::~CLRenderer()
	{ std::puts("Finishing all calls..."); }


	void CLRenderer::Clear(COLORREF col)
	{
		cl_int ec;
		ec = clearKernel.setArg(1, col); CHECK_EC(ec);

		ec = q.enqueueNDRangeKernel(clearKernel, cl::NullRange, cl::NDRange(wnd->Height)); CHECK_EC(ec);
		ec = q.flush(); CHECK_EC(ec);
		ec = q.finish(); CHECK_EC(ec);
	}

	void CLRenderer::Draw()
	{
		if (verts.empty())
			return;

		cl_int ec;
		for (size_t i = 0; i < verts.size(); i++)
		{
			vertMem = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, verts[i].size() * sizeof(Vertex), verts[i].data(), &ec); CHECK_EC(ec);
			indiceMem = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, indices[i].size() * sizeof(Indice), indices[i].data(), &ec); CHECK_EC(ec);
			ec = drawKernel.setArg(1, vertMem); CHECK_EC(ec);
			ec = drawKernel.setArg(2, indiceMem); CHECK_EC(ec);

			ec = q.enqueueNDRangeKernel(drawKernel, cl::NullRange, cl::NDRange(indices[i].size() / 3)); CHECK_EC(ec);
			ec = q.flush(); CHECK_EC(ec);
		}
		ec = q.finish(); CHECK_EC(ec);

		verts.clear();
		indices.clear();
	}

	void CLRenderer::Render(std::vector<Vertex> verts, std::vector<Indice> indices)
	{
		this->verts.push_back(verts);
		this->indices.push_back(indices);
	}


	bool CLRenderer::IsAlive()
	{ return alive; }

	bool CLRenderer::IsDrawing()
	{ return !verts.empty(); }

	inline const char *clGetErrorString(cl_int ec)
	{
		switch (ec)
		{
		// run-time and JIT compiler errors
		case 0: return "CL_SUCCESS";
		case -1: return "CL_DEVICE_NOT_FOUND";
		case -2: return "CL_DEVICE_NOT_AVAILABLE";
		case -3: return "CL_COMPILER_NOT_AVAILABLE";
		case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
		case -5: return "CL_OUT_OF_RESOURCES";
		case -6: return "CL_OUT_OF_HOST_MEMORY";
		case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
		case -8: return "CL_MEM_COPY_OVERLAP";
		case -9: return "CL_IMAGE_FORMAT_MISMATCH";
		case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
		case -11: return "CL_BUILD_PROGRAM_FAILURE";
		case -12: return "CL_MAP_FAILURE";
		case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
		case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
		case -15: return "CL_COMPILE_PROGRAM_FAILURE";
		case -16: return "CL_LINKER_NOT_AVAILABLE";
		case -17: return "CL_LINK_PROGRAM_FAILURE";
		case -18: return "CL_DEVICE_PARTITION_FAILED";
		case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

		// compile-time errors
		case -30: return "CL_INVALID_VALUE";
		case -31: return "CL_INVALID_DEVICE_TYPE";
		case -32: return "CL_INVALID_PLATFORM";
		case -33: return "CL_INVALID_DEVICE";
		case -34: return "CL_INVALID_CONTEXT";
		case -35: return "CL_INVALID_QUEUE_PROPERTIES";
		case -36: return "CL_INVALID_COMMAND_QUEUE";
		case -37: return "CL_INVALID_HOST_PTR";
		case -38: return "CL_INVALID_MEM_OBJECT";
		case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
		case -40: return "CL_INVALID_IMAGE_SIZE";
		case -41: return "CL_INVALID_SAMPLER";
		case -42: return "CL_INVALID_BINARY";
		case -43: return "CL_INVALID_BUILD_OPTIONS";
		case -44: return "CL_INVALID_PROGRAM";
		case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
		case -46: return "CL_INVALID_KERNEL_NAME";
		case -47: return "CL_INVALID_KERNEL_DEFINITION";
		case -48: return "CL_INVALID_KERNEL";
		case -49: return "CL_INVALID_ARG_INDEX";
		case -50: return "CL_INVALID_ARG_VALUE";
		case -51: return "CL_INVALID_ARG_SIZE";
		case -52: return "CL_INVALID_KERNEL_ARGS";
		case -53: return "CL_INVALID_WORK_DIMENSION";
		case -54: return "CL_INVALID_WORK_GROUP_SIZE";
		case -55: return "CL_INVALID_WORK_ITEM_SIZE";
		case -56: return "CL_INVALID_GLOBAL_OFFSET";
		case -57: return "CL_INVALID_EVENT_WAIT_LIST";
		case -58: return "CL_INVALID_EVENT";
		case -59: return "CL_INVALID_OPERATION";
		case -60: return "CL_INVALID_GL_OBJECT";
		case -61: return "CL_INVALID_BUFFER_SIZE";
		case -62: return "CL_INVALID_MIP_LEVEL";
		case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
		case -64: return "CL_INVALID_PROPERTY";
		case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
		case -66: return "CL_INVALID_COMPILER_OPTIONS";
		case -67: return "CL_INVALID_LINKER_OPTIONS";
		case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

		// extension errors
		case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
		case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
		case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
		case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
		case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
		case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
		default: return "Unknown OpenCL error";
		}
	}
}