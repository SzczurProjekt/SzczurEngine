#version 330 core



// Vertex
layout (location = 0) in vec3 _position;
layout (location = 1) in vec4 _color;
layout (location = 2) in vec2 _texCoord;

// Fragment
out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentTexCoord;



// Main shader funciton
void main() {
	fragmentPosition = _position;
    fragmentColor    = _color;
    fragmentTexCoord = _texCoord;
    
	gl_Position = vec4(_position, 1.0);
}


