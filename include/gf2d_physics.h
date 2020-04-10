typedef struct Collider_S{
    float x,y;
    float width, height;
}Collider;
typedef struct Physics_S
{
    float       gravity;
    Collider    box;
}Physics;

