#version 150

const int NO_LIGHT = 0, GOURAUD = 1, PHONG = 2;
const int HEAD = 0, BODY = 1, UPPER_LEG = 2, LOWER_LEG = 3, TAIL = 4;


//하나하나의 element를 가져와서 처리해줌 -> 계속 바뀜

in vec4 vPosition;
in vec4 vNormal;
in vec2 vHeadTexCoord;
in vec2 vBodyTexCoord;
in vec2 vUpperLegTexCoord;
in vec2 vLowerLegTexCoord;
in vec2 vTailTexCoord;

out vec4 fragPos;
out vec4 color;
out vec4 normal;
out vec2 texCoord;

//모든 vertex에 대해 동일한 하나의 variable을 건내줌
//uniform mat4 mPVM;
uniform mat4 mProject;
uniform mat4 mView; 
uniform mat4 mModel;

uniform int shadeMode;
uniform int isTexture;
uniform int animalPart;

void main() 
{
	gl_Position = mProject * mView * mModel * vPosition;

	vec4 vColor = vec4(0, 0, 1, 1);
	if(isTexture == 1){
		vColor = vec4(1, 1, 1, 1);
	}
	vec4 L = normalize(vec4(3, 3, 5, 0));
	float kd = 0.8, ks = 1.0, ka = 0.2, shininess = 60;
	vec4 Id = vColor;
	vec4 Is = vec4(1, 1, 1, 1);
	vec4 Ia = vColor;

	if (shadeMode == NO_LIGHT)
	{
		color = vColor;
	}
	else if (shadeMode == GOURAUD)
	{
		// ambient
		float ambient = ka;

		// diffuse
		normal = transpose(inverse(mModel)) * vNormal;
		vec4 N = normalize(normal);
		float diff = kd * clamp(dot(N, L), 0, 1);

		// specular
		vec4 viewPos = inverse(mView) * vec4(0, 0, 0, 1);
		vec4 worldPos = mModel * vPosition;
		vec4 V =  normalize(viewPos - worldPos);
		vec4 R = reflect(-L, N);
		float spec = ks * pow(clamp(dot(V, R), 0, 1), shininess);

		color = ambient * Ia + diff * Id + spec * Is;
	}
	else // if (shadeMode == PHONG)
	{
		fragPos = mModel * vPosition;
		normal = transpose(inverse(mModel)) * vNormal;
		color = vColor;
	}

	if (isTexture == 1) {
		if (animalPart == HEAD){
			texCoord = vHeadTexCoord;
		}

		else if (animalPart == BODY){
			texCoord = vBodyTexCoord;
		}

		else if (animalPart == UPPER_LEG){
			texCoord = vUpperLegTexCoord;
		}

		else if (animalPart == LOWER_LEG){
			texCoord = vLowerLegTexCoord;
		}
		
		else{
			texCoord = vTailTexCoord;
		}
	}

} 
