#pragma once

#include "../sdk/signatures/signatures.h"

// CMultiPlayerAnimState::Update
// one of them is it
ADD_SIG(CTFPlayerAnimState_Update, "client.so", "55 31 C0 48 89 E5 41 57 41 56 41 55 66 41 0F 7E CD")

void Hooked_CTFPlayerAnimState_Update(void* thisptr, float eyeYaw, float eyePitch);
void Hook_CTFPlayerAnimState_Update();