# Colors Library

### > GetAimbotTargetColor( )
returns **table** [r: **int**, g: **int**, b: **int**, a: **int**]

### > GetRedTeamColor( )
returns **table** [r: **int**, g: **int**, b: **int**, a: **int**]

### > GetBluTeamColor( )
returns **table** [r: **int**, g: **int**, b: **int**, a: **int**]

### > GetWeaponColor( )
returns **table** [r: **int**, g: **int**, b: **int**, a: **int**]

## Examples

Get target color
```lua
local color = colors.GetAimbotTargetColor()
print(color.r, color.g, color.b, color.a) --- output: 255 255 255 255
```