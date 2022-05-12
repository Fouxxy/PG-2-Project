#version 460 core

// outputs
// layout ( location = 0 ) out vec4 FragColor;
out vec4 FragColor;

// inputs
in vec3 vertex_normal;
in vec3 position_ws;
in vec2 texCoords;
in mat3 TBN;

uniform vec3 cameraPosition;


struct Material {
	sampler2D albedo;
	sampler2D normal;
	sampler2D metallic;
	sampler2D roughness;
	sampler2D AO;	
};

struct Environment {
    sampler2D EXR_irradiance;
    sampler2D EXR_prefiltered;
    sampler2D EXR_GGX;
};

uniform samplerCube envCubemap;

struct Light {
    vec3 position;   
    vec3 color;
  };


//uniform int useTexture_diffuse = 0;
//uniform int useTexture_specular = 0;
uniform Light light;  
uniform Material material;
uniform Environment environment;
const float PI = 3.14159265359;

vec2 cartesianToSphericalToUV(vec3 N)
{
    float dx = N.x;
    float dy = N.y;
    float dz = N.z;
    vec2 UV = vec2(0.5 + atan(dx, dz)/(2 * PI), 0.5 + asin(dy)/PI);
    return UV;
}

// TODO understand this
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normal, texCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(position_ws);
    vec3 Q2  = dFdy(position_ws);
    vec2 st1 = dFdx(texCoords);
    vec2 st2 = dFdy(texCoords);

    vec3 N  = normalize(vertex_normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

void main( void )
{	
    // Prevod do linear rgb
	vec3 albedo     = pow(texture(material.albedo, texCoords).rgb, vec3(2.2));
	//vec3 albedo     = pow(texture(material.albedo, texCoords).rgb, vec3(2.2));
    //vec3 normal     = getNormalFromMap();
    //float metallic  = texture(material.metallic, texCoords).g;
    float metallic  = 1.0;
    //float roughness = texture(material.roughness, texCoords).r;
    float roughness = 0.2;
    float AO        = texture(material.AO, texCoords).b;

    //vec3 N = (texture(material.normal, texCoords).rgb * 2.0 - 1.0) * vertex_normal;       
    //vec3 N = (texture(material.normal, texCoords).rgb) * vertex_normal;       
    //N = normalize(TBN * N);
    
    vec3 N = getNormalFromMap();
    //vec3 N = vertex_normal;
    vec3 V = normalize(cameraPosition - position_ws);
    vec3 R = reflect(-V, N); 
    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);


    // Light Equation
    vec3 Lo = vec3(0.0);
    vec3 L = normalize(light.position - position_ws);
    vec3 H = normalize(V + L);
    float distance = length(light.position - position_ws);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
    vec3 numerator    = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;
        
    // kS is equal to Fresnel
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
   
    kD *= 1.0 - metallic;	  

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);        

    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

    //vec3 ambient = vec3(0.03) * albedo * AO;
    vec2 UV_N = cartesianToSphericalToUV(N);
    vec2 UV_R = cartesianToSphericalToUV(R);
    
    

    vec3 F_R = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(environment.EXR_prefiltered, UV_R,  roughness * MAX_REFLECTION_LOD).rgb;    
    
    vec2 brdf  = texture(environment.EXR_GGX, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 spec = prefilteredColor * (F_R * brdf.x + brdf.y);
   
   
   
    vec3 irradiance = texture(envCubemap, N).rgb;
   
    vec3 diff = irradiance * albedo;
    //vec3 ambient =  irradiance * albedo * AO;
    vec3 ambient =  (kD * diff + spec) * AO;
    
    
    vec3 color = ambient + Lo;



    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correction
    color = pow(color, vec3(1.0/2.2)); 
   // color = N * 0.5 + 0.5;
    //color = vertex_normal * 0.5 + 0.5;

    FragColor = vec4(color, 1.0);
}
