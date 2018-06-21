#include "GlyphArea.hpp"

#include "../GlyphTypes.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

namespace rat
{
    GlyphArea::GlyphArea(GrayPPArea& source)
    :
    _sourceArea(source),
    BaseBar([]{ auto* base = new ListWidget; base->makeHorizontal(); base->setAutoBetweenPadding(); return base;}())
    {
        setPropOrigin(0.5f, 0.f);
        setPropSize(0.6f, 0.f);

        _container.addGlyph(GlyphID::Wrath, 2);
        _container.addGlyph(GlyphID::Wearines, 1);
        _container.addGlyph(GlyphID::Fear, 3);
        _container.addGlyph(GlyphID::Desperation, 0);

        GlyphTypes types;
        size_t i = 0;
        GlyphesConverter converter;
        for(auto& type : types)
        {
            auto& glyphBar = _glyphBars[i];
            glyphBar = std::make_unique<GlyphBar>(source);
            glyphBar->setType(converter.toEnum(type));
            _addBar(glyphBar);
            i++;
        }
    }

    void GlyphArea::initAssetsViaGUI(GUI& gui)
    {
        for(auto& glyphBar : _glyphBars)
        {
            glyphBar->initAssetsViaGUI(gui);
        }
    }
}