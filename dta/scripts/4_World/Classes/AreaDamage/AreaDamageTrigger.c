class AreaDamageTrigger extends Trigger
{
	protected int				m_TriggerUpdateMs;
	protected vector			m_ExtentMin;
	protected vector			m_ExtentMax;
	
	protected AreaDamageBase	m_AreaDamageType;
	
	void AreaDamageTrigger()
	{
		m_TriggerUpdateMs = 100;
		m_AreaDamageType = null;
	}

	void ~AreaDamageTrigger()
	{
		//! call OnLeave for all insiders when removing trigger
		for(int n = 0; n < m_insiders.Count(); )
		{
			TriggerInsider ins = m_insiders.Get(n);
			Object insObj = ins.GetObject();
			if( insObj )
			{
				//object left. Remove it
				OnLeave(ins.insider.Ptr());
				m_insiders.Remove(n);
				continue;
			}
			 
			n++;
		}
	}
	
	override protected void UpdateInsiders(int timeout)
	{
		vector max = GetPosition() + m_ExtentMax;
		vector min = GetPosition() + m_ExtentMin;

		for(int n = 0; n < m_insiders.Count(); )
		{
			TriggerInsider ins = m_insiders.Get(n);
			if( ins.insider == null )
			{
				//object has been deleted. Remove it
				m_insiders.Remove(n);
				continue;
			}

			Object insObj = ins.GetObject();
			if( insObj && vector.Distance(insObj.GetPosition(), GetPosition()) > (GetRadius(m_ExtentMin, m_ExtentMax) / 2) + 0.2 )
			{
				if(g_Game.GetTime() - ins.timeStamp > 500)
				{
					//object left. Remove it
					OnLeave(ins.insider.Ptr());
					m_insiders.Remove(n);
					continue;
				}
			}
			 
			n++;
		}
	}

	override void AddInsider(Object obj)
	{
		TriggerInsider ins;
		if( obj )
		{
			for(int n = 0; n < m_insiders.Count(); n++)
			{
				ins = m_insiders.Get(n);
				//already in?
				if( obj && ins.GetObject() == obj )
				{
					//just update timestamp
					//Print("Already in");
					ins.timeStamp = g_Game.GetTime();
					return;
				}
			}
	
			ins = new TriggerInsider(obj);
			ins.timeStamp = g_Game.GetTime();
			m_insiders.Insert(ins);
			OnEnter(obj);
		}
	}

	override void SetExtents(vector mins, vector maxs)
	{
		m_ExtentMax = maxs;
		m_ExtentMin = mins;

		super.SetExtents(mins, maxs);
	}
	
	override void OnEnter( Object obj )
	{
		super.OnEnter( obj );

		//if( GetGame().IsServer() )
		{
			//obj.OnAreaDamageEnter();

			if( m_AreaDamageType )
			{
			 	m_AreaDamageType.OnEnter(obj);
				//Print("On Enter called!");
			}
		}
	}
	
	override void OnLeave(Object obj)
	{
		super.OnLeave( obj );

		//if( GetGame().IsServer() )
		{
			//obj.OnAreaDamageLeave();

			if( m_AreaDamageType )
			{
		 		m_AreaDamageType.OnLeave(obj);
				//Print("On Leave called!");
			}
		}
	}
	
	void SetAreaDamageType(AreaDamageBase adType)
	{
		m_AreaDamageType = adType;
	}
}