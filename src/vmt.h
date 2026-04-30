#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <cstring>

class VMT
{
public:
	VMT() = default;
	~VMT() 
	{ 
		Restore(); 
	}

	bool Setup(void* object, size_t explicit_size = 0) 
	{
		if (!object) 
			return false;

		target_object = reinterpret_cast<void***>(object);
		original_vtable = *target_object;

		if (explicit_size > 0) 
			vtable_size = explicit_size;
		else 
			vtable_size = EstimateTableSize(original_vtable);

		if (vtable_size == 0) 
			return false;

		shadow_vtable = std::make_unique<void*[]>(vtable_size + 1);

		std::memcpy(shadow_vtable.get(), original_vtable - 1, (vtable_size + 1) * sizeof(void*));

		*target_object = shadow_vtable.get() + 1;

		return true;
	}

	template<typename T>
	void Hook(size_t index, T detour) 
	{
		if (index >= vtable_size || !shadow_vtable) 
			return;
		
		// +1 because shadow_vtable[0] is the RTTI pointer in our array
		shadow_vtable[index + 1] = reinterpret_cast<void*>(detour);
	}

	void Unhook(size_t index) 
	{
		if (index >= vtable_size || !shadow_vtable || !original_vtable) 
			return;
		
		shadow_vtable[index + 1] = original_vtable[index];
	}

	template<typename T>
	T GetOriginal(size_t index) 
	{
		if (!original_vtable) 
			return nullptr;
		
		return reinterpret_cast<T>(original_vtable[index]);
	}

	void Restore() 
	{
		if (target_object && original_vtable) 
		{
			*target_object = original_vtable;
			target_object = nullptr;
		}
	}

private:
	void*** target_object = nullptr;
	void** original_vtable = nullptr;
	std::unique_ptr<void*[]> shadow_vtable;
	size_t vtable_size = 0;

	size_t EstimateTableSize(void** vtable) 
	{
		size_t size = 0;

		while (vtable[size] != nullptr && size < 1024)
			size++;

		return size;
	}
};