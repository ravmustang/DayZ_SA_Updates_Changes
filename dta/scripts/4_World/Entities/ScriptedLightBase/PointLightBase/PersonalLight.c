class PersonalLight extends PointLightBase
{	
	void PersonalLight()
	{
		SetVisibleDuringDaylight(false);
		SetRadiusTo( 15 );
		SetBrightnessTo(0.2);
		SetDiffuseColor(0.1, 0.3, 0.6);
		SetCastShadow(false);
		EnableSpecular(false);
		EnableLinear(true);
		SetFlareVisible(false);
		Print("PersonalLight created");
	}
	
	void ~PersonalLight()
	{
		Print("PersonalLight destroyed");
	}
	
	override void OnFrameLightSource(IEntity other, float timeSlice)
	{
		vector pos = GetGame().GetCurrentCameraPosition();
		vector dir = GetGame().GetCurrentCameraDirection();
		
		SetPosition(pos);
	}
}