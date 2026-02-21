# Material Library

## Methods

### > CreateMaterial( name: **string**, vmt: **string** )
returns **Material**

The **vmt** parameter must be valid

Example:
```lua
local mat = materials.Create("test",
[[
UnlitGeneric
{
	$basetexture "white"
}
]])
```

### > FindMaterial( name: **string**, groupName: **string**, [complain: **bool** = true] )
returns **Material?**

### > FindTexture( name: **string**, groupName: **string**, [complain: **bool** = true] )
returns **Texture?**

### > CreateTextureRenderTarget( name: **string**, width: **int**, height: **int** )
returns **Texture?**

## Examples

Create a material and use it
```lua
local mat = MaterialManager.CreateMaterial("myfirstmaterial",
[[UnlitGeneric
{
	$basetexture "white"
}]])

hooks.Add("DrawModel", "lolo", function(ctx: DrawModelContext)
	if ctx.entity and ctx.entity:IsPlayer() then
		render.ForcedMaterialOverride(mat)
		render.SetColorModulation(1, 0, 0)
	end
end)
```