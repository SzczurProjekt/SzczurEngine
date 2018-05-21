#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

class BattlePawn;
class InputManager;

class BattleSkill {
public:

// Constructors

	///
	BattleSkill(BattlePawn* pawn, const std::string& name);
	
	///
	BattleSkill(const BattleSkill& obj);

// Main

	/// Init when skill is created
	virtual void init();

	/// Update when skill is activated
	virtual void update(float deltaTime);
	
// Getters

	///
	bool isFinished();

	///
	BattlePawn* getPawn();

	///
	const std::string& getName() const;

	///
	size_t getType() const;

	///
	bool isUsable() const;
	
	///
	sf::Sprite getIconSprite() const;

// Manipulations

	///
	void kill();

	/// Example: SELECT_ENEMY|SELECT_OBJECT
	void setType(size_t type);

// Controller

	///
	void renderCircle(sf::RenderTarget& canvas, float radius);

	///
	void renderController(sf::RenderTarget& canvas);

	///
	void updateController(BattlePawn* selectedPawn);

	///
	void setUsable(int icon);

// Scripts

	///
	void setVariable(std::string key, sol::stack_object object);

	/// 
	sol::object getVariable(const std::string& key);

	///
	static void initScript(Script& script);

protected:

	BattlePawn* pawn = nullptr;

	/// Binary mask
	size_t selectType = SELECT_SPACE;

	bool killed = false;
	std::string name;
	bool usable = false;
	int icon = 0;

	InputManager& input;

// Scripts

	/// Non-C++ variables
	std::unordered_map<std::string, sol::object> data;

public:

	sol::function onInit;
	sol::function onUpdate;
	sol::function onProvide;

	enum SELECT_ {
		SELECT_SPACE = 1,
		SELECT_ENEMY = 1<<2,
		SELECT_FRIEND = 1<<3,
		SELECT_OBJECT = 1<<4,
		SELECT_SELF = 1<<5
	};

};

/*
	Script:
	> onSignal
*/

// class BattleSkillMove : public BattleSkill {
// public:

// 	///
// 	void init() override;
	
// 	///
// 	void update(float deltaTime) override;

// private:

// 	sf::Clock clock;
// 	sf::Vector2f posBegin;
// 	sf::Vector2f posEnd;
// 	float angle;
// 	float distance;
	
// public:

// 	// Signal after finish skill
// 	sol::function onSignal;

// };

}