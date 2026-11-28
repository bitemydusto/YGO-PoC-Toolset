#pragma once

#include "Player.h"

namespace Engine::Structures
{
    struct Duel
    {
        Player players[2];

        static Duel* getDuel()
        {
            return reinterpret_cast<Duel*>(Engine::Addresses::Data::GameState::PLAYER_LIST);
        }
    };
}