---@meta

--- CLASSES

---@class Vector3
---@field x number
---@field y number
---@field z number
---@operator add(Vector3): Vector3
---@operator sub(Vector3): Vector3
---@operator mul(number): Vector3
---@operator div(number): Vector3
---@operator unm(): Vector3

---@class Entity
local Entity = {}

---@return Vector3
function Entity:GetAbsOrigin() end

---@param newOrigin Vector3
function Entity:SetAbsOrigin(newOrigin) end

---@return Vector3
function Entity:GetAbsAngles() end

---@param newAngles Vector3
function Entity:SetAbsAngles(newAngles) end

---@return boolean
function Entity:IsPlayer() end

---@return boolean
function Entity:IsWeapon() end

---@return integer
function Entity:GetIndex() end

---@return Vector3
function Entity:GetCenter() end

---@return Vector3
function Entity:EstimateAbsVelocity() end

---@return boolean
function Entity:IsProjectile() end

---@return boolean
function Entity:SetupBones() end

---@param flags integer
function Entity:DrawModel(flags) end

---@return boolean
function Entity:ShouldDraw() end

---@return integer
function Entity:GetClassID() end

---@return integer
function Entity:GetTeamNumber() end

---@return string
function Entity:GetClassName() end

---@return Player?
function Entity:ToPlayer() end

---@return Weapon?
function Entity:ToWeapon() end

---@return Entity?
function Entity:ToEntity() end

---@param attribute_name string
---@param default_value integer?
function Entity:AttributeHookInt(attribute_name, default_value) end

---@param attribute_name string
---@param default_value integer?
function Entity:AttributeHookFloat(attribute_name, default_value) end

---@return boolean
function Entity:IsValid() end

---@return boolean
function Entity:IsClientEntity() end

---@class Weapon: Entity
local Weapon = {}

---@return integer
function Weapon:GetID() end

---@return boolean
function Weapon:IsInReload() end

---@return boolean
function Weapon:CanPrimaryAttack() end

---@return boolean
function Weapon:CanSecondaryAttack() end

---@return boolean
function Weapon:IsMelee() end

---@return boolean
function Weapon:IsHitscan() end

---@return boolean
function Weapon:CanHitTeammates() end

---@return Vector3
function Weapon:GetDeflectionSize() end

---@return integer
function Weapon:GetType() end

---@return number
function Weapon:GetSmackTime() end

---@return number
function Weapon:GetChargeBeginTime() end

---@return WeaponData
function Weapon:GetData() end

---@class WeaponData
---@field m_nDamage integer
---@field m_nBulletsPerShot integer
---@field m_flRange number
---@field m_flSpread number
---@field m_flPunchAngle number
---@field m_flTimeFireDelay number
---@field m_flTimeIdle number
---@field m_flTimeIdleEmpty number
---@field m_flTimeReloadStart number
---@field m_flTimeReload number
---@field m_bDrawCrosshair boolean
---@field m_iProjectile integer
---@field m_iAmmoPerShot integer
---@field m_flProjectileSpeed number
---@field m_flSmackDelay number
---@field m_bUseRapidFireCrits boolean

---@class Player: Entity
local Player = {}

---@return boolean
function Player:IsAlive() end

---@return boolean
function Player:InCond() end

---@return boolean
function Player:IsGhost() end

---@return boolean
function Player:IsTaunting() end

---@return boolean
function Player:IsUbercharged() end

---@return Vector3
function Player:GetEyePos() end

---@return string
function Player:GetName() end

function Player:UpdateClientSideAnimation() end

---@return integer
function Player:GetWaterLevel() end

---@return number
function Player:GetInvisibilityLevel() end

---@return integer
function Player:GetUserID() end

---@return string
function Player:GetSteamID() end

---@return integer
function Player:GetSteamID3() end

---@param className string
---@param prop string
---@return integer
function Player:GetNetvarInt(className, prop) end

---@param className string
---@param prop string
---@return number
function Player:GetNetvarFloat(className, prop) end

---@param className string
---@param prop string
---@return Vector3
function Player:GetNetvarVector(className, prop) end

---@param className string
---@param prop string
---@return Entity?
function Player:GetNetvarEntity(className, prop) end

---@param className string
---@param prop string
---@param value integer
---@return integer
function Player:SetNetvarInt(className, prop, value) end

---@param className string
---@param prop string
---@param value number
---@return number
function Player:SetNetvarFloat(className, prop, value) end

---@param className string
---@param prop string
---@param value Vector3
---@return Vector3
function Player:SetNetvarVector(className, prop, value) end

---@param className string
---@param prop string
---@param value Entity
---@return Entity?
function Player:SetNetvarEntity(className, prop, value) end

---@class UserCmd
---@field command_number integer
---@field tick_count integer
---@field viewangles Vector3
---@field forwardmove number
---@field sidemove number
---@field upmove number
---@field buttons integer
---@field impulse integer
---@field weaponselect integer
---@field weaponsubtype integer
---@field random_seed integer
---@field mousedx integer
---@field mousedy integer
---@field hasbeenpredicted integer

--- LIBRARIES

hooks = {}

---@param event "CreateMove"|"ImGui"|"FireGameEvent"|"Draw"|"LevelInitPreEntity"|"LevelInitPostEntity"|"DoPostScreenSpaceEffects"|"OverrideView"|"FrameStageNotify"|"GameShutdown"|"DrawModel"
---@param id string
---@param func function
function hooks:add(event, id, func) end

---@param event "CreateMove"|"ImGui"|"FireGameEvent"|"Draw"|"LevelInitPreEntity"|"LevelInitPostEntity"|"DoPostScreenSpaceEffects"|"OverrideView"|"FrameStageNotify"|"GameShutdown"|"DrawModel"
---@param id string
function hooks:remove(event, id) end

engine = {}

---@param filepath string
function engine:play_sounds(filepath) end

---@return integer
function engine:get_max_clients() end

---@return Vector3
function engine:get_viewangles() end

---@param viewangles Vector3
function engine:set_viewangles(viewangles) end

---@return boolean
function engine:is_taking_screenshot() end

---@return boolean
function engine:is_in_game() end

---@param cmd string
---@param unrestricted boolean?
function engine:client_cmd(cmd, unrestricted) end

---@return boolean
function engine:is_gameui_visible() end

---@return boolean
function engine:is_console_visible() end

---@return boolean
function engine:is_connected() end

entities = {}

---@param entindex integer
function entities:get_client_entity(entindex) end

---@return integer
function entities:get_max_entities() end

---@return Player?
function entities:get_localplayer() end

IN_ATTACK = (1 << 0)
IN_JUMP = (1 << 1)
IN_DUCK = (1 << 2)
IN_FORWARD = (1 << 3)
IN_BACK = (1 << 4)
IN_USE = (1 << 5)
IN_CANCEL = (1 << 6)
IN_LEFT = (1 << 7)
IN_RIGHT = (1 << 8)
IN_MOVELEFT = (1 << 9)
IN_MOVERIGHT = (1 << 10)
IN_ATTACK2 = (1 << 11)
IN_RUN = (1 << 12)
IN_RELOAD = (1 << 13)
IN_ALT1 = (1 << 14)
IN_ALT2 = (1 << 15)
IN_SCORE = (1 << 16)
IN_SPEED = (1 << 17)
IN_WALK = (1 << 18)
IN_ZOOM = (1 << 19)
IN_WEAPON1 = (1 << 20)
IN_WEAPON2 = (1 << 21)
IN_BULLRUSH = (1 << 22)
IN_GRENADE1 = (1 << 23)
IN_GRENADE2 = (1 << 24)
IN_ATTACK3 = (1 << 25)

TF_CLASS_SCOUT = 1
TF_CLASS_SNIPER = 2
TF_CLASS_SOLDIER = 3
TF_CLASS_DEMOMAN = 4
TF_CLASS_MEDIC = 5
TF_CLASS_HEAVYWEAPONS = 6
TF_CLASS_PYRO = 7
TF_CLASS_SPY = 8
TF_CLASS_ENGINEER = 9

TFClass = {
	"Scout", 1,
	"Sniper", 2,
	"Soldier", 3,
	"Demoman", 4,
	"Medic", 4,
	"Heavyweapons", 5,
	"Pyro", 6,
	"Spy", 7,
	"Engineer", 8
}

FL_ONGROUND = (1 << 0)
FL_DUCKING = (1 << 1)
FL_WATERJUMP = (1 << 2)
FL_ONTRAIN = (1 << 3)
FL_INRAIN = (1 << 4)
FL_FROZEN = (1 << 5)
FL_ATCONTROLS = (1 << 6)
FL_CLIENT = (1 << 7)
FL_FAKECLIENT = (1 << 8)
FL_INWATER = (1 << 9)

---@class ConVar
local ConVar = {}

---@return integer
function ConVar:GetInt() end

---@return number
function ConVar:GetFloat() end

---@return string
function ConVar:GetString() end

---@return string
function ConVar:GetName() end

---@return string
function ConVar:GetHelpText() end

---@return boolean
function ConVar:IsCommand() end

---@param flag integer
---@return boolean
function ConVar:IsFlagSet(flag) end

---@param flags integer
function ConVar:AddFlags(flags) end

---@param value integer|number|string
function ConVar:SetValue(value) end

---@class PlayerInfo
---@field is_htlv boolean
---@field is_replay boolean
---@field is_bot boolean
---@field userid integer
---@field name string
---@field steamid string

client = {}

---@return PlayerInfo
function client.get_player_info(index) end

---@class Vector2
---@field x number
---@field y number

---@param world_pos Vector3
---@param out_pos Vector2
---@return boolean
function client.world_to_screen(world_pos, out_pos) end

---@return ConVar
function client.get_convar(name) end