class HeadtorchRedLight extends SpotLightBase
{
	void HeadtorchRedLight()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusClient(25);
		SetSpotLightAngle(130);
		SetCastShadow(true);
		SetBrightnessClient(6);
		SetAmbientColor(0.8, 0.1, 0.1);
		SetDiffuseColor(0.8, 0.1, 0.1);
	}
}