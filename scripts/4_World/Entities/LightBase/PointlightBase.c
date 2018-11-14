class PointlightBase extends LightBase
{
	void PointlightBase()
	{
		//SetEventMask(EntityEvent.FRAME); // use this in your child objects if you want to enable EOnFrame event

		SetLightType(LightSourceType.PointLight);
		SetVisibleDuringDaylight(true);
		SetRadius( 30 );
		SetCastShadow(true);
	}
	
	/*override void EOnFrame(IEntity other, float timeSlice)
	{
		
	}*/
}