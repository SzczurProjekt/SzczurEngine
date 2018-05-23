#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

class BattleScene;
class BattlePawn;

class BattleTrigger {
public:

// Constructors

	BattleTrigger(BattlePawn* caster);

// Main

	///
	void render(sf::RenderTarget& canvas);

	///
	void update(float deltaTime);

	///
	void init();

// Gettters

	///
	bool isFinished() const;

	///
	BattlePawn* getCaster();

	///
	const sf::Vector2f& getPosition() const;

	///
	bool isInited() const;

// Manipulations

	///
	void setRadius(float radius);
	
	///
	void setPosition(float x, float y);

	///
	void setPosition(const sf::Vector2f& pos);

	///
	void setDuration(float time);

	///
	void setCaster(BattlePawn* caster);

// Targeting 

	///
	void updateTargets();

	///
	void setType(const std::string& type);

	///
	void targetNearest();

	///
	BattlePawn* getTarget();

	///
	void nearestOnly();

	///
	void removeFromTargets(BattlePawn* pawn);

	///
	void replaceTargets(BattlePawn* pawn, BattlePawn* newPawn);

// Math

	///
	float getSemiDistanceTo(const sf::Vector2f& pos);

	///
	float getDistanceTo(const sf::Vector2f& pos);

// Scripts

	///
	void setVariable(std::string key, sol::stack_object object);

	///
	sol::object getVariable(const std::string& key);

	///
	static initScript(Script& script);

public:

	sol::function onInit;
	sol::function onUpdate;

private:

// Scripts

	/// Non-C++ variables
	std::unordered_map<std::string, sol::object> data;

// Main

	sf::Clock clock;
	float duration = 0.5f;
	BattlePawn* caster = nullptr;
	BattleScene* scene = nullptr;
	bool killed = false;
	bool inited = false;

// Visual

	sf::CircleShape shape;
	float radius = 10.f;
	sf::Vector2f position{0,0};

// Targeting

	bool nearestOnlyType = false;
	std::vector<BattlePawn*> targets;

};

}