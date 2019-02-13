class FlashlightLight extends SpotLightBase
{
	void FlashlightLight()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusClient(30);
		SetSpotLightAngle(120);
		SetCastShadow(true);
		EnableSpecular(true);
		SetBrightnessClient(6);
		SetAmbientColor(1.0, 0.85, 0.75);
		SetDiffuseColor(1.0, 0.85, 0.75);
	}
	
	override void OnFrameLightSource( IEntity other, float timeSlice)
	{
		//vector ori = GetOrientation();
		//Print(ori);
	}
}