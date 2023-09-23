struct input
{
    float4 pos : POSITION;
    float3 color : COLOR;
};
struct Output
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};
//Vertex shaders a responsilbe for processing each vertex before it is reasterized or put on the screen, They handle positioning and color of a vertex.
Output vsmain(input input)
{
    Output output;
    output.pos = input.pos;
    output.color = input.color;
    return output;
}