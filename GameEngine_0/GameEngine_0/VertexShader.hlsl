//Vertex shaders a responsilbe for processing each vertex before it is reasterized or put on the screen, They handle positioning and color of a vertex.
float4 vsmain(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}
