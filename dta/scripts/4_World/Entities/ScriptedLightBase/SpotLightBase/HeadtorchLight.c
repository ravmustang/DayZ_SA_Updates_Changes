class HeadtorchLight extends SpotLightBase
{
	void HeadtorchLight()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusClient(25);
		SetSpotLightAngle(130);
		SetCastShadow(true);
		SetBrightnessClient(6);
		SetAmbientColor(0.8, 0.9, 1.0);
		SetDiffuseColor(0.8, 0.9, 1.0);
	}
}