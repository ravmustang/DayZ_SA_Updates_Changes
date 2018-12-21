#include "dikCodes.h"

//For now, intel has no position nor name.
class Params
{
	//VARIABLE_NAME is defined by default.
	class OVERCAST
	{
		displayName = "Overcast"; //TODO: Stringtablize.
		type = number;
		subtype = range(0, 1);
		default = 0.5;
	};
	class OVERCAST_WANTED
	{
		displayName = "Overcast (desired)"; //TODO: Stringtablize.
		type = number;
		subtype = range(0, 1);
		default = 0.5;
	};
	class FOG
	{
		displayName = "Fog"; //TODO: Stringtablize.
		type = number;
		subtype = range(0, 1);
		default = 0;
	};
	class FOG_WANTED
	{
		displayName = "Fog (desired)"; //TODO: Stringtablize.
		type = number;
		subtype = range(0, 1);
		default = 0;
	};
	class DAY
	{
		displayName = "Day"; //TODO: Stringtablize.
		type = number;
		subtype = list(1, 31, 1);
		default = 10;
	};	
	class MONTH
	{
		displayName = "Month"; //TODO: Stringtablize.
		type = number;
		subtype = list(1, 12, 1);
		default = 5;
	};
	class YEAR
	{
		displayName = "Year"; //TODO: Stringtablize.
		type = number;
		default = 1985;
	};
	class HOUR
	{
		displayName = "Hours"; //TODO: Stringtablize.
		type = number;
		subtype = list(0, 23, 1);
		default = 7;
	};
	class MINUTE
	{
		displayName = "Minutes"; //TODO: Stringtablize.
		type = number;
		subtype = list(0, 55, 5);
		default = 30;
	};
};

shortcutKey[] =
{
	DIK_F7,
	" (F7)"
};

create[] =
{
	"[0,0] setOvercast %OVERCAST;",
	"[1800,0] setOvercast %OVERCAST_WANTED;",
	"[0,0] setFog %FOG;",
	"[1800,0] setFog %FOG_WANTED;",
	"setDate [%YEAR, %MONTH, %DAY, %HOUR, %MINUTE];"
};

update[] =
{
	"[0,0] setOvercast %OVERCAST;",
	"[1800,0] setOvercast %OVERCAST_WANTED;",
	"[0,0] setFog %FOG;",
	"[1800,0] setFog %FOG_WANTED;",
	"setDate [%YEAR, %MONTH, %DAY, %HOUR, %MINUTE];"
};

updatePosition[] = {};
destroy[] = {};

title = "Intel"; //TODO: Stringtablize.
// used as a template for the objects names
displayName = "Intel"; //TODO: Stringtablize.
visible = false;