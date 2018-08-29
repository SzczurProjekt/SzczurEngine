/** @file SF3DEventDispatcher.hpp
** @author Piotr Krupa (piotrkrupa06@gmail.com)
**/
#pragma once

#include <unordered_map>
#include <functional>
#include <vector>

#include <dragonBones/DragonBonesHeaders.h>

DRAGONBONES_NAMESPACE_BEGIN

class SF3DEventDispatcher : public IEventDispatcher
{
	using Func_t = std::function<void(EventObject*)>;

private:
	std::unordered_map<std::string, std::vector<Func_t>> _listeners;

public:
	SF3DEventDispatcher() = default;
	~SF3DEventDispatcher() = default; 

	void addDBEventListener(const std::string& type, const Func_t& listener) override
	{
		_listeners[type].push_back(listener);
	}

	void removeDBEventListener(const std::string& type, const Func_t& listener) override
	{
		// TOOD
	}

	void dispatchDBEvent(const std::string& type, EventObject* value) override
	{
		for (auto& listener : _listeners[type])
		{
			listener(value);
		}
	}

	bool hasDBEventListener(const std::string& type) const { return true; };
};

DRAGONBONES_NAMESPACE_END
