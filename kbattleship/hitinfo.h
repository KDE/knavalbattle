#ifndef HITINFO_H
#define HITINFO_H

class Ship;

struct HitInfo
{
    enum Type
    {
        HIT,
        MISS,
        INVALID
    };
    
    Type type;
    Ship* shipDestroyed;
    
    HitInfo(Type type, Ship* shipDestroyed = 0)
    : type(type)
    , shipDestroyed(shipDestroyed)
    {
    }
};

    
#endif // HITINFO_H

