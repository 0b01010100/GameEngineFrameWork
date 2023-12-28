Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};
//Retrive data is c++ version of the constant buffer
cbuffer constant : register(b0)
{
    float m_distortion_level;
};
float2 distort(float2 p)
{
    float theta = atan2(p.y, p.x); //Gets an Angle, in Polar coordinates based on the p.y, and p.x coordinates, which are relitive to the center of the screen.
    float radius = length(p);
    
    radius = pow(radius, m_distortion_level); //radius^m_distortion_level. modifies the distance of the point from the origin
    
//Polar Coordinates back Certesian Coordinates. 
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    
    return 0.5 * (p + 1.0); //Shifts and scales the distorted point to fit U-normalized range[0,1]
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 xy = 2.0 * input.texcoord.xy - 1; //calculates a new texture coordinate centered at (0,0) and scaled to fit within the range [-1, 1].
    float2 uv;
    float d = length(xy); //len of the current pixle from the center
    
    if (d < 1)//if distance from the center is less than 1
    {
        
        uv = distort(xy);
       // return float4(1, 0, 0, 1) * color;

    }
    else
    {
        uv = input.texcoord.xy;
        //return color;
    }
    
    float4 color = Texture.Sample(TextureSampler, uv);//Samples the texture to a Pixle
    return color;
}