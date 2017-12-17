#include <aurora/shaderSources.hpp>

const char* pointsVertexShaderSource = R"glsl(

)glsl";

const char* pointsGeometryShaderSource = R"glsl(

)glsl";

const char* pointsFragmentShaderSource = R"glsl(

)glsl";

// vertex shader source code
const char* squaresVertexShaderSource = R"glsl(
    #version 450 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aCol;

    out vec3 color;
    uniform float timeValue;
    uniform mat4 transform;

    vec3 genColor(in float t, in vec3 c, in vec3 p, in vec3 pt)
    {
        vec3 a;
        vec3 b;
        a = vec3(t*c.x+(1-t)*c.y,(1-t)*c.x+t*c.y,0.6*t*c.x+0.2*(1-c.y));
        b = vec3(t*pt.x+(1-t)*c.x,(1-t)*pt.y+t*c.y, 0.6*t+0.2*(1-p.y));

        if (t > 0.9)
        {
            if (int(t * 1000000) % 2 == 0)
            {
                return a;
            }
            else
            {
                return b;
            }
        }
     
        vec3 color;
        color = vec3(t*a.r+(1-t)*b.r, t*a.g+(1-t)*b.g, t*a.b+(1-t)*b.b);
        return color;
    }
            
    void main()
    {
        gl_Position = transform*vec4(aPos.xyz, 1.0);
        color = genColor(timeValue, aCol, aPos, gl_Position.xyz);
    }
)glsl";

// fragment shader source code
const char* squaresFragmentShaderSource = R"glsl(
    #version 450 core
    out vec4 fragColor;

    in vec3 color;

    void main()
    {
        fragColor = vec4(color.rgb, 1.0f);
    }
)glsl";


