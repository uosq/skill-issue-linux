#pragma once

#define ImGui_CheckboxBit(label, bitfield) \
[&]() { \
	bool temp = (bitfield); \
	bool changed = ImGui::Checkbox(label, &temp); \
	if (changed) bitfield = temp; \
	return changed; \
}()

#define ImGui_SliderIntBit(label, bitfield, v_min, v_max) \
[&]() { \
	int temp = (bitfield); \
	bool changed = ImGui::SliderInt(label, &temp, v_min, v_max); \
	if (changed) bitfield = temp; \
	return changed; \
}()
