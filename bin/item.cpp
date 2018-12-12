#include "dikCodes.h"

class Params
{
	//VARIABLE_NAME is defined by default.
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
		subtype = item;
		canChange = false;
	};
	class PARENT
	{
		displayName = "Parent"; //TODO: Stringtablize.
		source = parent;
		type = unit;
		subtype = single;
		shortcutKey = DIK_LALT;
	};
};

shortcutKey[] =
{
	DIK_F2,
	" (F2)"
};

create[] =
{
  "%VARIABLE_NAME = objNull;",
  "%!if (%PRESENCE_CONDITION) then {",
  "  if (typeName(%PARENT)=='ARRAY') then {onPlayerConnecting {%VARIABLE_NAME = playerOn _id createInInventory %TYPE}} else {%VARIABLE_NAME = %PARENT createInInventory %TYPE};",
  "};"
};

destroy[] =
{
	"_map deleteEditorObject ""%VARIABLE_NAME"";"
};

update[] =
{
	"%!if (!isNull %VARIABLE_NAME) then",
	"{",
  "  _this = %VARIABLE_NAME;",
  // TODO: handle parent change
	"};"
};

updatePosition[] = {};

title = "Item"; //TODO: Stringtablize.
// used as a template for the objects names
displayName = "%VARIABLE_NAME";
