//Retrive Earth texture
Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 direction_to_camera : TEXCOORD1;
};
//Retrive data is c++ version of the constant buffer
cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 m_light_direction;
    float4 m_camera_position;
    float4 m_light_position;
    float m_light_radius;
    float m_time = 0.0f;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    //Sample texture's data. This will allow us to get the color of every part of the texture and to 
    //help use assinge that color to the pixels that need it one the screen.
    float4 color = Color.Sample(ColorSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));


	//AMBIENT LIGHT
    float ka = 8.5;
    float3 ia = float3(0.09, 0.082, 0.082);
    ia *= (color.rgb);

    float3 ambient_light = ka * ia;

	//DIFFUSE LIGHT
    float kd = 0.7;

    float amount_diffuse_light = dot(m_light_direction.xyz, input.normal);
    float3 id = float3(1, 1, 1);
    id *= (color.rgb);
    
    float3 diffuse_light = kd * id * amount_diffuse_light;

	//SPECULAR LIGHT
    float ks = 0.0;
    float3 is = float3(1.0, 1.0, 1.0);
    float3 reflected_light = reflect(m_light_direction.xyz, input.normal);
    float shininess = 30.0;
    float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);

    float3 specular_light = ks * amount_specular_light * is;

    
    //PHONG SHADING MODLE
    float3 final_light = ambient_light + diffuse_light + specular_light;
    
    
    
    //DONE
    return float4(final_light, 1.0);
}