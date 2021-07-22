#include "Game/Block.h"

bool Block::IsSolid()
{
    return m_Type != Type::AIR;
}
