class SpotlightBase extends LightBase
{
	void SpotlightBase()
	{
		//SetEventMask(EntityEvent.FRAME); // use this in your child objects if you want to enable EOnFrame event

		SetLightType(LightSourceType.SpotLight);
		SetVisibleDuringDaylight(true);
		SetRadius( 30 );
		SetCastShadow(true);
	}
	
	/*override void EOnFrame(IEntity other, float timeSlice)
	{
		
	}*/
}