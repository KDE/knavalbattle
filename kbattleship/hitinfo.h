#ifndef HITINFO_H
#define HITINFO_H

struct HitInfo
{
    enum Type
    {
        HIT,
        MISS,
        INVALID
    };
    
    Type type;
    bool shipDestroyed;
    
    HitInfo(Type type, bool shipDestroyed = false)
    : type(type)
    , shipDestroyed(shipDestroyed)
    {
    }
};

    
#endif // HITINFO_H

