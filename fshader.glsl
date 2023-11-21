#version 150

const int NO_LIGHT = 0, GOURAUD = 1, PHONG = 2;
const int HEAD = 0, BODY = 1, UPPER_LEG = 2, LOWER_LEG = 3, TAIL = 4;

in vec4 fragPos;
in vec4 color;
in vec4 normal;
in vec2 texCoord;


out vec4  fColor;

uniform int shadeMode;
uniform mat4 mView;
uniform int isTexture;
uniform int animalPart;
uniform sampler2D headTexture;
uniform sampler2D bodyTexture;
uniform sampler2D upperLegTexture;
uniform sampler2D lowerLegTexture;
uniform sampler2D tailTexture;


void main() 
{ 
    vec4 L = normalize(vec4(3, 3, 5, 0));
	float kd = 0.8, ks = 1.0, ka = 0.2, shininess = 60;
	vec4 Id = color;
	vec4 Is = vec4(1, 1, 1, 1);
	vec4 Ia = color;

	if (shadeMode == NO_LIGHT)
	{
		if(isTexture == 1){
			if (animalPart == HEAD){
				fColor = texture( headTexture, texCoord ).rgba;
			}

			else if (animalPart == BODY){
				fColor = texture( bodyTexture, texCoord ).rgba;
			}

			else if (animalPart == UPPER_LEG){
				fColor = texture( upperLegTexture, texCoord ).rgba;
			}

			else if (animalPart == LOWER_LEG){
				fColor = texture( lowerLegTexture, texCoord ).rgba;
			}
		
			else{
				fColor = texture( tailTexture, texCoord ).rgba;
			}
		}
		else{
			fColor = color;
		}
	}
	else if (shadeMode == GOURAUD)
	{
		if(isTexture == 1){
			if (animalPart == HEAD){
				fColor = color * texture( headTexture, texCoord ).rgba;
			}

			else if (animalPart == BODY){
				fColor = color * texture( bodyTexture, texCoord ).rgba;
			}

			else if (animalPart == UPPER_LEG){
				fColor = color * texture( upperLegTexture, texCoord ).rgba;
			}

			else if (animalPart == LOWER_LEG){
				fColor = color * texture( lowerLegTexture, texCoord ).rgba;
			}
		
			else{
				fColor = color * texture( tailTexture, texCoord ).rgba;
			}
		}
		else{
			fColor = color;
		}
	}
	else // if (shadeMode == PHONG)
	{
		// ambient
		float ambient = ka;

		// diffuse
		vec4 N = normalize(normal);
		float diff = kd * clamp(dot(N, L), 0, 1);

		// specular
		vec4 viewPos = inverse(mView) * vec4(0, 0, 0, 1);
		vec4 V =  normalize(viewPos - fragPos);
		vec4 R = reflect(-L, N);
		float spec = ks * pow(clamp(dot(V, R), 0, 1), shininess);

		fColor = ambient * Ia + diff * Id + spec * Is;

		if(isTexture == 1){
			if (animalPart == HEAD){
				fColor = fColor * texture( headTexture, texCoord ).rgba;
			}

			else if (animalPart == BODY){
				fColor = fColor * texture( bodyTexture, texCoord ).rgba;
			}

			else if (animalPart == UPPER_LEG){
				fColor = fColor * texture( upperLegTexture, texCoord ).rgba;
			}

			else if (animalPart == LOWER_LEG){
				fColor = fColor * texture( lowerLegTexture, texCoord ).rgba;
			}
		
			else{
				fColor = fColor * texture( tailTexture, texCoord ).rgba;
			}
		}
	}
} 

