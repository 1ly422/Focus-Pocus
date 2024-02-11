#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition; 
in vec2 fragTexCoord;
in vec4 fragColor;
// Input uniform values
uniform sampler2D texture0;
uniform float pocusSize;
uniform vec2 mousePos;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables


void main() {
    // Calculate final fragment color

    float distanceToCenter = distance(fragPosition.xy , mousePos);
    
    if (distanceToCenter < pocusSize) 
        discard; // Discard the fragment (transparent)
    else 
        finalColor = fragColor; // Red color
}