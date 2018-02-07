#ifndef InputBeta_H
#define InputBeta_H
#include "json.hpp"
#include <SFML\System.hpp>
namespace Input {
	class InputBeta
	{
	public:
		InputBeta(nlohmann::json& j) { load(j); };
		~InputBeta();

		virtual void load(nlohmann::json& j)
		{
			m_status = j.at("status");
		};

	private:
		std::string m_status;
	};
}
#endif