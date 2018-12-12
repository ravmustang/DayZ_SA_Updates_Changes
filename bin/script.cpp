#include "dikCodes.h"

class Params
{
	//VARIABLE_NAME is defined by default.
	class TEXT
	{
		displayName = "Text"; //TODO: Stringtablize.
		type = code;
    subtype = long;
		default = "";
	};
  class PARENT
  {
    displayName = "Parent"; //TODO: Stringtablize.
    source = parent;
    type = unit;
    subtype = noneOrSingle;
    shortcutKey = DIK_LSHIFT;
  };
};

shortcutKey[] =
{
	DIK_F4,
	" (F4)"
};

create[] =
{
  "%!_script = {%TEXT};",
  "%!_executeForUnit = {if (typeName(_this select 0)=='ARRAY') then {onPlayerConnecting {playerOn _id call {%TEXT}}} else {(_this select 0) call {%TEXT}}};",
  "_parent = objNull;",
  "_parent = %PARENT;", // not done when there is no parent
  "%!if (""%PARENT"" != """") then {[_parent,_script] call _executeForUnit} else _script;"
};

destroy[] =
{
};

update[] =
{
  "%!_script = {%TEXT};",
  "_parent = objNull;",
  "_parent = %PARENT;", // not done when there is no parent
  "%!if (""%PARENT"" != """") then {(if (typeName _parent=='ARRAY') then {_parent select 0} else {_parent}) call _script} else _script;"
};

updatePosition[] = {};

title = "Script"; //TODO: Stringtablize.
// used as a template for the objects names
displayName = "%VARIABLE_NAME";
