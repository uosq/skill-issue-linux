# Materials Library

## Methods

### > Create( name: **string**, vmt: **string** )
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