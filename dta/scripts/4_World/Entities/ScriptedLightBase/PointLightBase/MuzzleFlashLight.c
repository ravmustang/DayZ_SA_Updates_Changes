class MuzzleFlashLight extends PointLightBase
{
	void MuzzleFlashLight()
	{
		SetLifetime(35);
		SetVisibleDuringDaylight(true);
		SetRadiusClient( 5 );
		SetBrightnessClient(1);
		SetCastShadow(true);
		SetAmbientColor(0.76, 0.68, 0.31);
		SetDiffuseColor(0.76, 0.68, 0.31);
	}
}