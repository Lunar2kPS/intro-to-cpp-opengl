#shader vertex
#version 330 core //GLSL version 330, core means it won't let us use any deprecated functions
layout(location = 0) in vec4 position;

void main() {
   gl_Position = position;
};

#shader fragment
#version 330 core //GLSL version 330, core means it won't let us use any deprecated functions
layout(location = 0) out vec4 color;

uniform vec4 uniColor;

void main() {
   color = uniColor;
};
