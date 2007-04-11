#include "shot.h"
#include "generalcontroller.h"

Shot::Shot(GeneralController* parent, Sea::Player player, const Coord& pos)
: m_parent(parent)
, m_player(player)
, m_pos(pos)
{
}
    
void Shot::execute(const HitInfo& info) {
    m_parent->finalizeShot(m_player, m_pos, info);
}



