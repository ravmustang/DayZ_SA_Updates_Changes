class PointlightBase extends LightBase
{
	void PointlightBase()
	{
		//SetEventMask(EntityEvent.FRAME); // use this in your child objects if you want to enable EOnFrame event

		SetLightType(LightSourceType.PointLight);
		SetVisibleDuringDaylight(true);
		SetRadius( 30 );
		SetCastShadow(true);
		SetEnabled(true);
		SetDiffuseColor( 1, 1, 1 );
		SetAmbientColor( 1, 1, 1 );
		SetAmbientAlpha( 1 );
	}
	
	/*override void EOnFrame(IEntity other, float timeSlice)
	{
		
	}*/
}