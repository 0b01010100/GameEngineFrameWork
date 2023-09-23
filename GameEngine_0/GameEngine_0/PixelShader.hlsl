struct input
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;

};
//The Shader/Pixel shader handles the color calculations for each pixel on the screen by
//taking some data from the Vetex shader. 
float4 psmain(input input) : SV_Target
//SV_Taget tells us that the ouput will be stored in the first render target by native grahicp pipline/Ouput Merger stage
{
    
    return float4(input.color.r, input.color.g, input.color.b, 0.0f);
}
