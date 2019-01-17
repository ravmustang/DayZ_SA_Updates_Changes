class Light extends ItemBase
{
	ref Timer m_DeleteTimer;
	
	void Light()
	{
		SetPilotLight(true);
	}
	
	void SetLifetime(float lifetime)
	{
		m_DeleteTimer = new Timer( CALL_CATEGORY_SYSTEM );
		m_DeleteTimer.Run( lifetime , this, "OnDeleteThis", NULL, false);
	}
	
	void OnDeleteThis()
	{
		SetPilotLight(false);
		GetGame().ObjectDelete(this);
	}
}