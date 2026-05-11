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
function engine:PlaySound(filepath) end

---@return integer
function engine:GetMaxClients() end

---@return Vector3
function engine:GetViewAngles() end

---@param viewangles Vector3
function engine:SetViewAngles(viewangles) end

---@return boolean
function engine:IsTakingScreenshot() end

---@return boolean
function engine:IsInGame() end

---@param cmd string
---@param unrestricted boolean?
function engine:ClientCmd(cmd, unrestricted) end

---@return boolean
function engine:IsGameUIVisible() end

---@return boolean
function engine:IsConsoleVisible() end

---@return boolean
function engine:IsConnected() end

entities = {}

---@param entindex integer
function entities:GetClientEntity(entindex) end

---@return integer
function entities:GetMaxEntities() end

---@return Player?
function entities:GetLocalPlayer() end