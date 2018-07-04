#include "Window.hpp"

/** @file Window.cpp
 ** @description Implementation file with main class of the Window module.
 ** @author Patryk (Stritch)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/Vertex.hpp"
#include "Szczur/Utility/SFML3D/Shader.hpp"
#include "Szczur/Utility/SFML3D/ShaderProgram.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat
{

/* Properties */
// Window
Window::Window_t& Window::getWindow()
{
	return this->window;
}
const Window::Window_t& Window::getWindow() const
{
	return this->window;
}

// VideoMode
sf::VideoMode Window::getVideoMode() const
{
	return this->videoMode;
}
void Window::setVideoMode(const sf::VideoMode& mode)
{
	this->videoMode = mode;
	LOG_INFO("VideoMode: { width: ", this->videoMode.width,  ", height: ", this->videoMode.height, ", bitsPerPixel: ", this->videoMode.bitsPerPixel, " }");

	this->recreateWindow();
}

// FrameRate
unsigned int Window::getFramerateLimit() const
{
	return this->framerateLimit;
}
void Window::setFramerateLimit(const unsigned int limit)
{
	this->framerateLimit = limit;
	this->getWindow().setFramerateLimit(this->framerateLimit);
}

// Title
const std::string& Window::getTitle() const
{
	return this->title;
}
void Window::setTitle(const std::string& title)
{
	this->getWindow().setTitle(title);
}

// Fullscreen
bool Window::getFullscreen() const
{
	return this->windowStyle & sf::Style::Fullscreen;
}
void Window::setFullscreen(bool state)
{
	if (state) {
		this->windowStyle = sf::Style::Fullscreen;
	}
	else {
		this->windowStyle = sf::Style::Default;
	}
	this->recreateWindow();
}


/* Operators */
// Constructors
Window::Window()
	: window(
		{1280, 800}, "Szczur3D"
		// @todo ! tu brakuje tego ShaderProgram dla Window :|
	) // @warn @todo . usunąc to i używać init
{
	LOG_INFO("Initializing Window module");
	this->init();
	LOG_INFO("Module Window initialized");
}
// Destructor
Window::~Window()
{
	LOG_INFO("Module Window destructed");
}



/* Methods*/
// init
void Window::init()
{
	// Create
	// @todo ? load videomode from settings
	this->setVideoMode(this->videoMode);

	// Print OpenGL version
	LOG_INFO("OpenGL version: ", GLVersion.major, ".", GLVersion.minor);
	
	// GL flags
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);  
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	// Default shaders
	sf3d::FShader frag;
	frag.loadFromFile("Assets/Shaders/post_processing.frag");

	sf3d::VShader vert;
	vert.loadFromFile("Assets/Shaders/post_processing.vert");

	this->shaderProgram = std::make_unique<sf3d::ShaderProgram>();
	this->shaderProgram->linkShaders(frag, vert);
	this->getWindow().setDefaultShaderProgram(shaderProgram.get());

	sf3d::FShader pFrag;
	pFrag.loadFromFile("Assets/Shaders/default.frag");

	sf3d::VShader pVert;
	pVert.loadFromFile("Assets/Shaders/default.vert");

	pShaderProgram.linkShaders(pFrag, pVert);
	auto size = window.getSize();
	renderTexture.create(&pShaderProgram, {size.x, size.y});

	renderTextureSprite = std::make_unique<sf3d::SimpleSprite>();

	renderTextureSprite->setTexture(renderTexture.getTexture());
}

// render
void Window::render()
{
	glDisable(GL_DEPTH_TEST);
	this->getWindow().draw(*renderTextureSprite);
	glEnable(GL_DEPTH_TEST);
	this->getWindow().display();
}

// Window recreate
void Window::recreateWindow()
{
	this->getWindow().create(this->videoMode, this->title, this->shaderProgram.get(), this->windowStyle);
}

// clear
void Window::clear(const sf::Color& color)
{
	renderTexture.clear(color, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	window.clear(color, GL_COLOR_BUFFER_BIT);
}

// GL states
void Window::pushGLStates()
{
	this->getWindow().pushGLStates();
}
void Window::popGLStates()
{
	this->getWindow().popGLStates();
}

// draw
// 2D
void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	this->getWindow().draw(drawable, states);
}
void Window::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
{
	this->getWindow().draw(vertices, vertexCount, type, states);
}
// 3D
void Window::draw(const sf3d::Drawable& drawable, const sf3d::RenderStates& states)
{
	renderTexture.draw(drawable, states);
}
void Window::draw(const sf3d::Drawable& drawable)
{
	renderTexture.draw(drawable);
}
void Window::draw(const sf3d::VertexArray& vertices, const sf3d::RenderStates& states)
{
	renderTexture.draw(vertices, states);
}
void Window::draw(const sf3d::VertexArray& vertices)
{
	renderTexture.draw(vertices);
}

}
