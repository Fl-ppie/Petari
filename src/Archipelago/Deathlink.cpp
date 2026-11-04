#include "Archipelago/Deathlink.hpp"
#include "Archipelago/NetworkHandler.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioHolder.hpp"
#include "Game/Scene/GameScene.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    bool canMarioMove() {
        return MR::getMarioHolder()->getMarioActor()->isEnableMoveMario();
    }

    bool canPause() {
        GameSystemSceneController* pSceneController = SingletonHolder< GameSystem >::get()->mSceneController;
        if (!MR::isEqualString(pSceneController->mCurrSceneControlInfo.mScene, "Game")) {
            return false;
        }
        
        return const_cast< GameScene* >(reinterpret_cast< GameScene* >(pSceneController->mScene))->isPermitToPauseMenu();
    }

};  // namespace

Deathlink::Deathlink() : mDeathTimer(), mDeathBuffer() {
}

void Deathlink::update() {
    if (mDeathTimer == 0) {
        if (mDeathBuffer > 0 && ::canPause() && MR::isExistMario() && ::canMarioMove()) {
            MarioAccess::getPlayerActor()->forceGameOver();

            mDeathTimer = DEATH_TIMEOUT;
            mDeathBuffer--;
        }
    } else {
        mDeathTimer--;
    }

    if (mDeathTimer == 0 && mDeathBuffer == 0 && ::canPause() && MR::isExistMario() && MR::isPlayerConfrontDeath()) {
        sendDeath();
    }
}

void Deathlink::recvDeath() {
    mDeathBuffer++;
}

void Deathlink::sendDeath() {
    SingletonHolder< NetworkHandler >::get()->sendPacket(APPacket(PacketType_Deathlink, 0));
}
