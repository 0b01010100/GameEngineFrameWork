//HLSL
//The Shader/Pixel shader handles the color calculations for each pixel on the screen by
//taking some data from the Vetex shader. 
float4 psmain(float4 pos : SV_POSITION) : SV_Target
{
    return float4(6.f, 3.f, 0.f, 1.0f);
}
