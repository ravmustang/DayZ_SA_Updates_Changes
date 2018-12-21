class CCTMan : CCTBase
{
	protected float m_MaximalActionDistance;
	protected bool m_MustBeAlive;
	
	void CCTMan ( float maximal_target_distance, bool must_be_alive = true )
	{
		m_MaximalActionDistance = maximal_target_distance;	
		m_MustBeAlive = must_be_alive;
	}
	
	override bool Can( PlayerBase player, ActionTarget target )
	{	
		if( !target )
		{
			return false;
		}
		
		Object targetObject = target.GetObject();
		if ( player && targetObject && targetObject != player && targetObject.IsMan() )
		{
			PlayerBase man = PlayerBase.Cast(targetObject);
			if ( man )
			{
				float distance = Math.AbsFloat(vector.Distance(man.GetPosition(),player.GetPosition()));
				if (  distance <= m_MaximalActionDistance && player.IsFacingTarget(targetObject) )
				{
					if( m_MustBeAlive )
					{
						if( man.IsDamageDestroyed() )
						{
							return false;
						}
						else
						{
							return true;
						}
					}
					return true;
				}
			}
		}
		
		return false;
	}
};