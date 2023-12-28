//Retrive Earth texture
Texture2D EarthColor : register(t0);
sampler EarthColorSampler : register(s0);
//Retrive EarthSpecular texture
Texture2D EarthSpecular : register(t1);
//Since there are multiple ways to sample textures, we need to know how to sazzmple. 
//This varaible hold the information on how to sample the texture.
sampler EarthSpecularSampler : register(s1);
//Retrive Clouds texture
Texture2D Clouds : register(t2);
//Since there are multiple ways to sample textures, we need to know how to sample. 
//This varaible hold the information on how to sample the texture.
sampler CloudsSampler : register(s2);
//Retrive EarthNight texture
Texture2D EarthNight : register(t3);
//Since there are multiple ways to sample textures, we need to know how to sample. 
//This varaible hold the information on how to sample the texture.
sampler EarthNightSampler : register(s3);
//Custon dat type to store the position, texcoord, normal, and direction_to_camera 
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
    float m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    //Sample texture's data. This will allow us to get the color of every part of the texture and to 
    //help use assinge that color to the pixels that need it one the screen.
    float4 earth_color = EarthColor.Sample(EarthColorSampler, 1.0 - input.texcoord);
    float earth_spec = EarthSpecular.Sample(EarthSpecularSampler, 1.0 - input.texcoord).r;
    float clouds = Clouds.Sample(CloudsSampler, 1.0 - input.texcoord + float2(m_time / 100.0, 0)).r;
    float4 earth_night = EarthNight.Sample(EarthNightSampler, 1.0 - input.texcoord);

	//AMBIENT LIGHT
    float ka = 1.5;
    float3 ia = float3(0.09, 0.082, 0.082);
    ia *= (earth_color.rgb);

    float3 ambient_light = ka * ia;

	//DIFFUSE LIGHT
    float kd = 0.7;
    float3 id_day = float3(1.0, 1.0, 1.0);
    id_day *= (earth_color.rgb + clouds);

    float3 id_night = float3(1.0, 1.0, 1.0);
    id_night *= (earth_night.rgb + clouds * 0.3);

    float amount_diffuse_light = dot(m_light_direction.xyz, input.normal);

    float3 id = lerp(id_night, id_day, (amount_diffuse_light + 1.0) / 2.0);//Lerp from night to day

    float3 diffuse_light = kd * id;

	//SPECULAR LIGHT
    float ks = earth_spec;
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