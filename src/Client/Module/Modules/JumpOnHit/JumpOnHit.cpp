#include "JumpOnHit.hpp"

#include "Client.hpp"
#include "SDK/Client/Network/Packet/EntityEventPacket.hpp"

void JumpOnHit::onEnable() {
    Listen(this, PacketEvent, &JumpOnHit::onPacketReceive)
    Module::onEnable();
}

void JumpOnHit::onDisable() {
    Deafen(this, PacketEvent, &JumpOnHit::onPacketReceive)
    Module::onDisable();
}

void JumpOnHit::defaultConfig() {
    Module::defaultConfig("core");
    setDef("jump_strength", 0.42f);
    setDef("only_on_ground", false);
}

void JumpOnHit::settingsRender(float settingsOffset) {
    initSettingsPage();

    addSlider("Jump Strength", "How high to jump when hit", "jump_strength", 1.0f, 0.1f);
    addToggle("Only On Ground", "Only jump when standing on ground", "only_on_ground");

    FlarialGUI::UnsetScrollView();
    resetPadding();
}

void JumpOnHit::onPacketReceive(PacketEvent& event) {
    if (!this->isEnabled()) return;
    if (!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer()) return;

    auto player = SDK::clientInstance->getLocalPlayer();

    if (event.getPacket()->getId() == MinecraftPacketIds::ActorEvent) {
        auto entityEventPkt = (EntityEventPacket*)event.getPacket();

        // Cek apakah event Hurt dan targetnya adalah local player
        if (entityEventPkt->EventID == ActorEvent::Hurt &&
            entityEventPkt->RuntimeID == player->getRuntimeIDComponent()->runtimeID) {

            auto* stateVec = player->getStateVectorComponent();
            if (!stateVec) return;

            // Kalau setting "only_on_ground" aktif, skip kalau player lagi di udara
            if (getOps<bool>("only_on_ground") && !player->isOnGround()) return;

            float strength = getOps<float>("jump_strength");
            stateVec->velocity.y = strength;
        }
    }
}
