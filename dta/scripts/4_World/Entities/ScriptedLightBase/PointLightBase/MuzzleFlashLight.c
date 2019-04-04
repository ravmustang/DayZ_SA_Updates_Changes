class MuzzleFlashLight extends PointLightBase
{
	void MuzzleFlashLight()
	{
		SetLifetime(0.035);
		SetVisibleDuringDaylight(true);
		SetRadiusTo( 5 );
		SetBrightnessTo(1);
		SetCastShadow(false);
		SetAmbientColor(0.76, 0.68, 0.31);
		SetDiffuseColor(0.76, 0.68, 0.31);
		SetFlareVisible(false);
	}
}