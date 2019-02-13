class Headtorch_Grey : Headtorch_ColorBase 
{
	override ScriptedLightBase CreateHeadtorchLight()
	{
		return ScriptedLightBase.CreateLight(HeadtorchLight, "0 0 0", 60);
	}
};