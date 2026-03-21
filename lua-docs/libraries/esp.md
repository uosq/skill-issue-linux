# ESP Library

### > Register( id: **string**, text: **string**, alignment: **int**, color: {r: **int**, g: **int**, b: **int**, a: **int**}, func(entity: **Entity**, data: **ESP_Data**): **bool** )
returns **bool**

The function needs to return a true or false. When its true, it will draw the text.

The possible values of the **alignment** parameter:

| Name | Value |
| - | - |
| ESP_ALIGN_INVALID | -1 |
| ESP_ALIGN_LEFT | 0 |
| ESP_ALIGN_RIGHT | 1 |
| ESP_ALIGN_TOP | 2 |
| ESP_ALIGN_BOTTOM | 3 |
| ESP_ALIGN_MAX | 4 |

### > Unregister( id: **string** )
returns **bool**

### > UnregisterAll( )
Unregisters every lua element from the ESP

## Examples

```lua
local col = {r = 255, g = 0, b = 255, a = 255}

esp.Register
(
	"coolid",
	"hello",
	ESP_ALIGN_TOP,
	col,
	function(entity: Entity, data: ESP_Data)
		return entity:IsAlive() and data.health > 0
	end
)
```