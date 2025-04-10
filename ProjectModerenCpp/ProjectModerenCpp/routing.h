#pragma once
#include <crow.h>

namespace http {
	class Routing 
	{
	public:
		void Run();
	private:
		crow::SimpleApp m_app;
	};
}
