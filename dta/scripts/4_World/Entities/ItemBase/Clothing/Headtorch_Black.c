class Headtorch_Black : Headtorch_ColorBase 
{
	override ScriptedLightBase CreateHeadtorchLight()
	{
		return ScriptedLightBase.CreateLight(HeadtorchRedLight, "0 0 0", 60);
	}
}