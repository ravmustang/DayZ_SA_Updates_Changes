class Flashlight extends ItemBase
{
	override void OnWorkStart()
	{
		SetPilotLight(true);
	}

	override void OnWorkStop()
	{
		SetPilotLight(false);
	}
}