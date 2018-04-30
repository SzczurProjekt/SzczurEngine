#include "GUI.hpp"
#include <iostream>
#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "ScrollAreaWidget.hpp"
namespace rat {
    GUI::GUI() 
    :
    _standartWindowSize(getModule<Window>().getWindow().getSize())
    {
        LOG_INFO(this, "Module GUI constructed")
        initScript();
        auto& window = getModule<Window>().getWindow();
        _canvas.create(window.getSize().x, window.getSize().y);
    }

    void GUI::initScript() {
        Script& script = getModule<Script>();
        auto module = script.newModule("GUI");
        module.set_function("addInterface", &GUI::addInterface, this);
        module.set_function("getTexture", &GUI::getAsset<sf::Texture>, this);
        module.set_function("getFont", &GUI::getAsset<sf::Font>, this);

        module.set_function("addTexture", &GUI::addAsset<sf::Texture>, this);
        module.set_function("addFont", &GUI::addAsset<sf::Font>, this);




        script.initClasses<Widget, ImageWidget, TextWidget, TextAreaWidget, ScrollAreaWidget>();
        //script.scriptFile("data/_GUI.lua");
    }

    GUI::~GUI() {
        LOG_INFO(this, "Module GUI destructed")
    }

    Widget* GUI::addInterface() {
        Widget* widget = new Widget;
        _root.add(widget);
        return widget;
    }
    
    void GUI::input(const sf::Event& event) {
        if(event.type == sf::Event::Resized)
        {
            sf::Vector2f winProp = { float(event.size.width) / float(_standartWindowSize.x),
            float(event.size.height) / float(_standartWindowSize.y) };
            Widget::setWinProp(winProp);
            std::cout << "Blala\n";
        }
        _root.input(event);
    }

    void GUI::update(float deltaTime) {
        _root.update(deltaTime);
    }

    void GUI::render() {
        _canvas.clear(sf::Color::Transparent);
        
        _canvas.draw(_root);

        _canvas.display();
        getModule<Window>().getWindow().draw(sf::Sprite(_canvas.getTexture()));
    }
}