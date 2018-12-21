class Params
{
  //VARIABLE_NAME is defined by default.
  class ADDON
  {
    type = text;
    subtype = multiple;
  };
};

create[] =
{
  "%!activateAddons %ADDON;",
};

update[] =
{
  "%!activateAddons %ADDON;"
};

updatePosition[] = {};
destroy[] = {};

title = "Prefix";  //TODO: Stringtablize.
// used as a template for the objects names
displayName = "Prefix"; //TODO: Stringtablize.