#include "dikCodes.h"

class Params
{
	//VARIABLE_NAME is defined by default.
	class POSITION
	{
		source = position;
		type = vector;
	};
	class PRESENCE_CONDITION
	{
		displayName = "Presence condition"; //TODO: Stringtablize.
		type = condition;
		default = "true";
	};
	class TYPE
	{
		displayName = "Type"; //TODO: Stringtablize.
		type = special;
		subtype = agent;
		canChange = false;
	};	
	class MARKER
	{
		source = link;
		type = marker;
		subtype = multiple; 
	};
	class PLACEMENT
	{
		displayName = "Placement radius"; //TODO: Stringtablize.
		type = number;
		default = 0;
	};
	class SPECIAL
	{
		displayName = "Special setting"; //TODO: Stringtablize.
		type = enum;
		subtype = ArcadeUnitSpecial;
		default = "CAN_COLLIDE";
	};
	class AZIMUT
	{
		source = direction;
		type = number;
		default = 0;
	};
	class NAME
	{
		displayName = "Name"; //TODO: Stringtablize.
		type = varname;
		default = "";
	};
	class HEALTH
	{
		displayName = "Health"; //TODO: Stringtablize.
		type = number;
		subtype = range(0, 1);
		default = 1;
	};
	class AMMO
	{
		displayName = "Ammunition"; //TODO: Stringtablize.
		type = number;
		subtype = range(0, 1);
		default = 1;
	};
	class INIT
	{
		displayName = "Init expression"; //TODO: Stringtablize.
		type = expression;
		default = "";
	};
	class SKILL
	{
		displayName = "Skill"; //TODO: Stringtablize.
		type = number;
		subtype = range(0.2, 1);
		default = 0.6;
	};
	class COMMANDER
	{
		source = link;
		type = vehicle;
		subtype = noneOrSingle;
	};
	class DRIVER
	{
		source = link;
		type = vehicle;
		subtype = noneOrSingle;
	};
	class GUNNER
	{
		source = link;
		type = vehicle;
		subtype = noneOrSingle;
	};
	class CARGO
	{
		source = link;
		type = vehicle;
		subtype = noneOrSingle;
	};
  class PLAYER
  {
    displayName = "Player"; //TODO: Stringtablize.
    type = bool;
    subtype = (STR_PLAYER, STR_PLAYER_AI);
    default = "false";
    // if no player exist, set new agent as a player
    onInit = "if (isNil '_this') then {private ['_found']; _found = false; {if (_map evalObjectArgument [_x, 'PLAYER']) then {_found = true;}} forEach (_map listObjects 'UNIT'); if (!_found) then {_control_player lbSetCurSel 1} else {_control_player lbSetCurSel 0;_control_player ctrlEnable false}}";
  };
};

shortcutKey[] =
{
	DIK_F1,
	" (F1)"
};

create[] =
{
  "%VARIABLE_NAME = objNull;",
  "_createUnit = ",
  "{",
  "  %!_this = createAgent [%TYPE, %POSITION, %MARKER, %PLACEMENT, %SPECIAL];",
  "  %VARIABLE_NAME = if (%PLAYER) then {[_this]} else {_this};",
  "  _this setDir %AZIMUT;",
  "  _this setVehicleVarName ""%NAME"";",
  "  %NAME = _this;",
  "  _this setVehicleArmor %HEALTH;",
  "  _this setVehicleAmmo %AMMO;",
  "  _this setVehicleInit %INIT;",
  "  _this setUnitAbility %SKILL;",
  "  _this moveInCommander %COMMANDER;",
  "  _this moveInDriver %DRIVER;",
  "  _this moveInGunner %GUNNER;",
  "  _this moveInCargo %CARGO;",
  "  if (%PLAYER) then {_id selectPlayer _this};",
  "};",
  // variable initialized as empty array so that item creation can reference it before any unit is created (happens on dedicated server)
  "%!if (%PLAYER) then {%VARIABLE_NAME = [];onPlayerConnecting _createUnit;} else {if (%PRESENCE_CONDITION) then _createUnit};",
};

destroy[] =
{
  "%!if (%PLAYER) then {selectNoPlayer;deleteVehicle (%VARIABLE_NAME select 0)}", // deleteVehicle cannot delete the player
  "else {deleteVehicle %VARIABLE_NAME};"
};

update[] =
{
  "if (typeName(%VARIABLE_NAME)=='ARRAY' && !%PLAYER) then {selectNoPlayer;%VARIABLE_NAME = %VARIABLE_NAME select 0};", // handling changing Player->AI
  "if (typeName(%VARIABLE_NAME)!='ARRAY' && %PLAYER) then {selectPlayer %VARIABLE_NAME;%VARIABLE_NAME = [%VARIABLE_NAME] select 0};", // handling changing Player->AI
  "_updateUnit = ",
  "{",
  "  _this setDir %AZIMUT;",
  "  _this setVehicleVarName ""%NAME"";",
  "  %NAME = _this;", // avoid for empty %NAME
  "  _this setVehicleArmor %HEALTH;",
  "  _this setVehicleAmmo %AMMO;",
  "  _this setUnitAbility %SKILL;",
  "  moveOut _this;",
  "  _this moveInCommander %COMMANDER;",
  "  _this moveInDriver %DRIVER;",
  "  _this moveInGunner %GUNNER;",
  "  _this moveInCargo %CARGO;",
  "};",
  "%!if (%PLAYER) then {{_x call _updateUnit} forEach players} else {if (!isNull %VARIABLE_NAME) then {%VARIABLE_NAME call _updateUnit}};",
};

//No updatePosition - proxy == object.
updatePosition[] = {};

position = "position if (%PLAYER) then {%VARIABLE_NAME select 0} else {%VARIABLE_NAME}";
proxy = "if (%PLAYER) then {%VARIABLE_NAME select 0} else {%VARIABLE_NAME}";

title = "Character";  //TODO: Stringtablize.
// used as a template for the objects names
displayName = "%VARIABLE_NAME: %NAME (%TYPE)";

drawMap[] =
{
  "{",
  "  _map drawLine [%POSITION, _map evalObjectArgument [_x, ""POSITION""], [0, 1, 1, _alpha]];",
  "} forEach %(MARKER);",
  "if (""%DRIVER"" != """") then",
  "{",
  "  _map drawLine [_map evalObjectArgument [""%DRIVER"", ""POSITION""], %POSITION, [1, 0, 1, _alpha]];",
  "};",
  "if (""%COMMANDER"" != """") then",
  "{",
  "  _map drawLine [_map evalObjectArgument [""%COMMANDER"", ""POSITION""], %POSITION, [1, 0, 0.5, _alpha]];",
  "};",
  "if (""%GUNNER"" != """") then",
  "{",
  "  _map drawLine [_map evalObjectArgument [""%GUNNER"", ""POSITION""], %POSITION, [0.5, 0, 1, _alpha]];",
  "};",
  "if (""%CARGO"" != """") then",
  "{",
  "  _map drawLine [_map evalObjectArgument [""%CARGO"", ""POSITION""], %POSITION, [0.5, 0, 0.5, _alpha]];",
  "};",
  "private ""_color"";",
  "_color = [0, 0, 1, _alpha];",
  "_map drawIcon [getText (configFile >> ""CfgVehicles"" >> %TYPE >> ""icon""), _color, %POSITION, 24, 24, %AZIMUT];",
  "if (%PLACEMENT > 0) then",
  "{",
  "  _map drawEllipse [%POSITION, %PLACEMENT, %PLACEMENT, 0, _color];",
  "};",
  "if (%PLAYER) then",
  "{",
  "  _map drawIcon [getText (configFile >> ""CfgInGameUI"" >> ""IslandMap"" >> ""iconPlayer""), [0.7, 0.1, 0, _alpha], %POSITION, 16, 16, 0];",
  "};"
};