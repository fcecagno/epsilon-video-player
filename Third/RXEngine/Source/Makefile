all: Camera color Definitions mat3 mat4 matcc Mathematics vec2 vec3 vec3d vec4
	g++ -shared -Wl *.o -o libRXEngine.so

CFLAGS = -I ../Inc/RX -fPIC

Camera: Camera.cpp
	g++ $(CFLAGS) -c Camera.cpp

color: color.cpp
	g++ $(CFLAGS) -c color.cpp

Definitions: Definitions.cpp
	g++ $(CFLAGS) -c Definitions.cpp

mat3: mat3.cpp
	g++ $(CFLAGS) -c mat3.cpp

mat4: mat4.cpp
	g++ $(CFLAGS) -c mat4.cpp

matcc: matcc.cpp
	g++ $(CFLAGS) -c matcc.cpp

Mathematics: Mathematics.cpp
	g++ $(CFLAGS) -c Mathematics.cpp

vec2: vec2.cpp
	g++ $(CFLAGS) -c vec2.cpp

vec3: vec3.cpp
	g++ $(CFLAGS) -c vec3.cpp

vec3d: vec3d.cpp
	g++ $(CFLAGS) -c vec3d.cpp

vec4: vec4.cpp
	g++ $(CFLAGS) -c vec4.cpp

vec4d: vec4d.cpp
	g++ $(CFLAGS) -c vec4d.cpp
