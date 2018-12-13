class SymptomAnimHandler
{
	int m_HCLevel = -1;//-1 none | 0 cold | 1 hot
	PlayerBase m_Player;
	protected ref HumanMovementState m_MovementState = new HumanMovementState();
	
	void SymptomAnimHandler(PlayerBase player)
	{
		m_Player = player;
	}
	
	void Update(float deltaT, HumanMovementState hms)
	{
		if( m_HCLevel != -1 )
		{
			m_Player.StartCommand_Modifier(m_HCLevel);
			m_HCLevel = -1;
		}
	}
	
	void SetCommandParams(int level)
	{
		m_HCLevel = level;
	}
	
	
}