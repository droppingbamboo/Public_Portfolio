#version 440 

in vec3 vertexNormal; // For incoming normals
in vec3 vertexFragmentPos; // For incoming fragment position
in vec2 vertexTextureCoordinate;

out vec4 fragmentColor; // For outgoing cube color to the GPU

// Uniform / Global variables for object color, light color, light position, and camera/view position
uniform vec3 objectColor;
uniform vec3 keyLightColor;
uniform vec3 fillLightColor;
uniform vec3 keyLightPos;
uniform vec3 fillLightPos;
uniform vec3 viewPosition;
uniform sampler2D uTexture; // Useful when working with multiple textures
uniform vec2 uvScale;

void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    //Calculate Ambient lighting*/
    float fillAmbientStrength = 0.1f; // Set ambient/global lighting strength for fill light
    float keyAmbientStrength = 0.5f;  // Set ambient/globlal lighting strength for key light
    vec3 fillAmbient = fillAmbientStrength * fillLightColor; // Generate ambient light color from fill lamp
    vec3 keyAmbient = keyAmbientStrength * keyLightColor;    // Generate ambient light color from key lamp
    vec3 ambient = keyAmbient + fillAmbient;   //combine for overall effect

    //Calculate Diffuse lighting*/
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
    vec3 keyLightDirection = normalize(keyLightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
    vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos); 

    float keyImpact = max(dot(norm, keyLightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    float fillImpact = max(dot(norm, fillLightDirection), 0.0);

    vec3 keyDiffuse = keyImpact * keyLightColor; // Generate diffuse light color
    vec3 fillDiffuse = fillImpact * fillLightColor;
    vec3 diffuse = keyDiffuse + fillDiffuse;

    //Calculate Specular lighting*/
    float specularIntensity = 0.4f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
    vec3 reflectDir = reflect(-keyLightDirection, norm);// Calculate reflection vector
    //Calculate specular component
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * keyLightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

    // Calculate phong result
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
}