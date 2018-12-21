#include "dikCodes.h"

class Params
{
	//VARIABLE_NAME is defined by default.
	class POSITION
	{
		source = position;
		type = vector;
	};
	class NAME
	{
		displayName = "Name"; //TODO: Stringtablize.
		type = text;
		canChange = false;
	};
	class TEXT
	{
		displayName = "Text"; //TODO: Stringtablize.
		type = text;
		default = "";
	};
	class MARKER_TYPE
	{
		displayName = "Shape"; //TODO: Stringtablize.
		type = enum;
		subtype = MarkerType;
		default = "ICON";
	};
	class TYPE
	{
		displayName = "Type"; //TODO: Stringtablize.
		type = config;
		subtype = CfgMarkers,name;
		default = "Flag";
	};
	class COLOR
	{
		displayName = "Color"; //TODO: Stringtablize.
		type = config;
		subtype = CfgMarkerColors,name;
		default = "Default";
	};
	class FILL
	{
		displayName = "Fill"; //TODO: Stringtablize.
		type = config;
		subtype = CfgMarkerBrushes,name;
		default = "Default";
	};
	class A
	{
		displayName = "Axis A (width)"; //TODO: Stringtablize.
		type = number;
		default = 1;
	};
	class B
	{
		displayName = "Axis B (height)"; //TODO: Stringtablize.
		type = number;
		default = 1;
	};
	class ANGLE
	{
		source = direction;
		type = number;
		default = 0;
	};
};

shortcutKey[] =
{
	DIK_F6,
	" (F6)"
};

create[] =
{
	"%!_this = createMarker [%NAME, %POSITION];",
	"_this setMarkerText %TEXT;",
	"_this setMarkerShape %MARKER_TYPE;",
	"%!_this setMarkerType %TYPE;",
	"_this setMarkerColor %COLOR;",
	"_this setMarkerBrush %FILL;",
	"_this setMarkerSize [%A, %B];",
	"_this setMarkerDir %ANGLE;",
	"%VARIABLE_NAME = _this;"
};

destroy[] =
{
	"deleteMarker %VARIABLE_NAME;"
};

update[] =
{
	"%VARIABLE_NAME setMarkerText %TEXT;",
	"%VARIABLE_NAME setMarkerShape %MARKER_TYPE;",
	"%VARIABLE_NAME setMarkerType %TYPE;",
	"%VARIABLE_NAME setMarkerColor %COLOR;",
	"%VARIABLE_NAME setMarkerBrush %FILL;",
	"%VARIABLE_NAME setMarkerSize [%A, %B];",
	"%VARIABLE_NAME setMarkerDir %ANGLE;"
};

updatePosition[] =
{
	"%VARIABLE_NAME setMarkerPos %POSITION;"
};

position = "markerPos %VARIABLE_NAME";

title = "Marker"; //TODO: Stringtablize.
// used as a template for the objects names
displayName = "Marker %NAME"; //TODO: Stringtablize.

//ToDo: create some 3D proxy object for in the 3D mode if necessary.
//proxy = """OFP2Test_Fur_Toilet_A"" createVehicle %POSITION";

drawMap[] =
{
	"if (%MARKER_TYPE == ""ELLIPSE"") then",
	"{",
	"  _color = getArray (configFile >> ""CfgMarkerColors"" >> %COLOR >> ""color"");",
	"  _color set [3, _alpha * (_color select 3)];",
	"  _fill = getText (configFile >> ""CfgMarkerBrushes"" >> %FILL >> ""texture"");",
	"  _map drawEllipse [%POSITION, %A, %B, %ANGLE, _color, _fill];",
	"  ",
	"}",
	"else",
	"{",
	"  if (%MARKER_TYPE == ""RECTANGLE"") then",
	"  {",
	"    _color = getArray (configFile >> ""CfgMarkerColors"" >> %COLOR >> ""color"");",
	"    _color set [3, _alpha * (_color select 3)];",
	"    _fill = getText (configFile >> ""CfgMarkerBrushes"" >> %FILL >> ""texture"");",
	"    _map drawRectangle [%POSITION, %A, %B, %ANGLE, _color, _fill];",
	"  }",
	"  else",
	"  {",
	"    _icon = getText (configFile >> ""CfgMarkers"" >> %TYPE >> ""icon"");",
	"    private ""_color"";",
	"    if (%COLOR == ""Default"") then",
	"    {",
	"      _color = getArray (configFile >> ""CfgMarkers"" >> %TYPE >> ""color"");",
	"    }",
	"    else",
	"    {",
	"      _color = getArray (configFile >> ""CfgMarkerColors"" >> %COLOR >> ""color"");",
	"    };",
	"    _color set [3, _alpha * (_color select 3)];",
	"    _map drawIcon [_icon, _color, %POSITION, %A * 20, %B * 20, %ANGLE, %TEXT, true];",
	"  };",
	"};"
};