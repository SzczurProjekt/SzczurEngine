#include "Widget.hpp"
#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
    template<typename... Ts>
    void basicScript(ScriptClass<Ts...>& object) {
        auto basicValues = [](Widget* widget, sol::table tab){
        if(tab["position"].valid()) {
            widget->setPosition(
                tab["position"][1],
                tab["position"][2]
            );
        }
        if(tab["visible"].valid()) {
            if(!tab["visible"])
                widget->invisible();
        }
        if(tab["active"].valid()) {
            if(!tab["active"])
                widget->deactivate();
        }
        if(tab["onHover"].valid())
            widget->setLuaCallback(Widget::CallbackType::onHover, tab["onHover"]);

        if(tab["onHoverIn"].valid())
            widget->setLuaCallback(Widget::CallbackType::onHoverIn, tab["onHoverIn"]);

        if(tab["onHoverOut"].valid())
            widget->setLuaCallback(Widget::CallbackType::onHoverOut, tab["onHoverOut"]);

        if(tab["onPress"].valid())
            widget->setLuaCallback(Widget::CallbackType::onPress, tab["onPress"]);

        if(tab["onHold"].valid())
            widget->setLuaCallback(Widget::CallbackType::onHold, tab["onHold"]);

        if(tab["onRelease"].valid())
            widget->setLuaCallback(Widget::CallbackType::onRelease, tab["onRelease"]);
        };

        object.setProperty(
            "visible",
            [](Widget& owner){ return owner.isVisible(); },
            [](Widget& owner, bool value){ (value) ? owner.visible() : owner.invisible();}
        );

        object.setProperty(
            "active",
            [](Widget& owner){ return owner.isActivated(); },
            [](Widget& owner, bool value){ (value) ? owner.activate() : owner.deactivate();}
        );

        object.setProperty(
            "position",
            [](Widget& owner){ return owner.getPosition(); },
            [](Widget& owner, sol::table tab){ owner.setPosition( tab[1], tab[2] ); }
        );

        

        /*
        object.set(
            "addWidget",
            [basicValues](Widget& owner, sol::table tab){
                Widget* widget = new Widget;
                basicValues(widget, tab);
                owner.add(widget);
            }
        );
        */
        object.set(
            "addWidget",
            [basicValues](Widget& owner, sol::table tab){
                Widget* widget = new Widget;
                basicValues(widget, tab);
                owner.add(widget);
                return widget;
            }
        );

        object.set(
            "addTextArea",
            [basicValues](Widget& owner, sol::table tab){
                if(tab["font"].valid()) {
                    TextAreaWidget* widget = new TextAreaWidget;
                    widget->setFont(tab["font"]);

                    if(tab["text"].valid())
                        widget->setString(tab["text"]);
                    
                    if(tab["fontSize"].valid())
                        widget->setCharacterSize(tab["fontSize"]);

                    auto size = tab["size"];
                    if(size.valid())
                        widget->setSize( sf::Vector2u{ size[1], size[2] } );
                    
                    auto color = tab["color"];
                    if(color.valid())
                        widget->setColor(sf::Color( color[1], color[2], color[3] ));

                    basicValues(widget, tab);
                    owner.add(widget);
                    return widget;
                }
            }
        );

        object.set(
            "addText",
            [basicValues](Widget& owner, sol::table tab){
                if(tab["font"].valid()) {
                    TextWidget* widget = new TextWidget;
                    widget->setFont(tab["font"]);

                    if(tab["text"].valid())
                        widget->setString(tab["text"]);
                    
                    if(tab["fontSize"].valid())
                        widget->setCharacterSize(tab["fontSize"]);
                    
                    auto color = tab["color"];
                    if(color.valid())
                        widget->setColor(sf::Color( color[1], color[2], color[3] ));

                    basicValues(widget, tab);
                    owner.add(widget);
                    return widget;
                }
            }
        );


        object.set(
            "addImage",
            [basicValues](Widget& owner, sol::table tab){
                if(tab["texture"].valid()) {
                    ImageWidget* widget = new ImageWidget;
                    widget->setTexture(tab["texture"]);

                    basicValues(widget, tab);
                    owner.add(widget);
                    return widget;
                }
            }
        );
    }
}