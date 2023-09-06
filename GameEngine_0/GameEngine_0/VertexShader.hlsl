float4 ChangePos(float4 position)
{
    if (position.y > 0)
    {
        position.x += 0.45f;
    }


    if (position.x > -1)
    {
        position.x -= 0.2f;
    }
    position.x += 0.1f;
        
        
    if (position.y == 0.5f)
    {
        position.y += 0.4f;
        position.x -= 0.2f;
            
    }
        
    position.y -= 0.2f;

    return position;
}
//Vertex shaders a responsilbe for processing each vertex before it is reasterized or put on the screen, They handle positioning and color of a vertex.
float4 vsmain(float4 pos : POSITION) : SV_POSITION
{
    return ChangePos(pos);
}