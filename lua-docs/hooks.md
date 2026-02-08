# Hooks

You can register a callback for a hook here.

They are called when the real function is being called, so you can modify the data or do something before it runs

### CalcViewModelView ( position: **Vector3**, angle: **Vector3** )
Called before drawing the Viewmodel

### LevelInitPreEntity ( mapName: **string** )
Called before receiving the first entity packet

### LevelInitPostEntity ( )
Called after receiving the first entity packet

### LevelShutdown ( )
Called when leaving a match

### CreateMove ( cmd: **UserCmd** )
Called when the game wants to create a movement command

### DoPostScreenSpaceEffects ( )
Called when the game wants to create screen effects

You can use this to make custom bloom, glow, etc

### Draw ( )
Called before drawing stuff to the screen

### DrawModel ( ctx: **DrawModelContext** )
Called before the game draws a model

You can use this to change how a model is drawn: their color, material, transparency, etc

### ImGui ( )
Called at the ImGui draw stage so you can make custom windows using the `ui` library

### SendNetMsg ( msg: **NetMessage** )
Called before sending a `NetMessage` to the server

You can use this to change the info we are sending to the server

### FrameStageNotify ( stage: **int** )
Called at various stages of a frame

### FireGameEvent ( event: **GameEvent** )
Called when a game event is called. For example, when a player spawns

### OverrideView ( view: **ViewSetup** )
Called before drawing the localplayer's view

### GameShutdown ( )
Called before the game closes so you can do stuff like saving settings or something else before closing

### ExecStringCmd ( cmd: **StringCmd** )
Called when trying to run a command in the console

## Examples

CalcViewModelView
```lua
local function PrintViewModelView(position: Vector3, angle: Vector3)
	print(position, angle)
end

hooks.Add("CalcViewModelView", "hello", PrintViewModelView)
```

LevelInitPreEntity
```lua
local function InitPreEntity(mapName: string)
	print($"Map is {mapName}")
end

hooks.Add("LevelInitPreEntity", "hi", InitPreEntity)
```

LevelInitPostEntity
```lua
local function InitPostEntity()
	print($"There are {globals.MaxClients()} players in the server right now")
end

hooks.Add("LevelInitPostEntity", "lol", InitPostEntity)
```

LevelShutdown
```lua
local function OnLeavingMatch()
	print("Leaving match :(")
end

hooks.Add("LevelShutdown", "sad", OnLeavingMatch)
```

CreateMove
```lua
local function OnCreateMove(cmd: UserCmd)
	local localplayer: Entity = entities.GetLocalPlayer()
	if localplayer == nil then
		return
	end

	print(localplayer:EstimateAbsVelocity())
end

hooks.Add("CreateMove", "damn", OnCreateMove)
```

DoPostScreenSpaceEffects
```lua
local function DPSSE()
	local players = entities.GetPlayers()
	if #players == 0 then
		return
	end

	for _, player in players do
		print(player:ShouldDraw())
	end
end

hooks.Add("DoPostScreenSpaceEffects", "lolo", DPSSE)
```

DrawModel
```lua
local function DrawModel(ctx: DrawModelContext)
	if ctx.entity and ctx.entity:IsPlayer() then
		render.SetColorModulation(1, 0, 0)
		render.SetBlend(0.5)
	end
end

hooks.Add("DrawModel", "beautiful", DrawModel)
```

ExecStringCmd
```lua
local function ExecStringCmd(cmd: StringCmd)
	local text = cmd:Get()
	if text == ""
end
```