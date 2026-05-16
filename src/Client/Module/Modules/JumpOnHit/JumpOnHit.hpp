#pragma once

#include "../Module.hpp"
#include "../../../Hook/Hooks/Game/PacketHooks.hpp"

class JumpOnHit : public Module {
public:
    JumpOnHit() : Module("Jump On Hit", "Automatically jumps when you get hit.",
        IDR_ARROW_PNG, "") {
    };

    void onEnable() override;
    void onDisable() override;
    void defaultConfig() override;
    void settingsRender(float settingsOffset) override;
    void onPacketReceive(PacketEvent& event);
};
