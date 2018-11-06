class BleedingSource
{
	vector m_Position;
	Particle m_BloodParticle;
	PlayerBase m_Player;
	int m_Bit;
	string m_Bone;
	ref BleedingSourceEffect m_BleedingEffect;
	vector m_Orientation;
	Shape m_DebugShape;
	Shape m_DebugShape1;
	ref Timer m_DebugTick;
	vector m_Offset;
	
	void BleedingSource(PlayerBase player, int bit, string bone, vector orientation, vector offset)
	{
		//m_Position = position;
		m_Player = player;
		m_Bit = bit;
		m_Bone = bone;
		m_Orientation = orientation;
		m_Offset = offset;
		//CreateBleedSymptom();
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
		{	
			CreateParticle();
		}
	}

	void ~BleedingSource()
	{
		if(m_BloodParticle)	RemoveParticle();
		if(m_DebugShape)
		{
			Debug.RemoveShape(m_DebugShape);
		}		
		
		if(m_DebugShape1)
		{
			Debug.RemoveShape(m_DebugShape1);
		}
		
		
	}
	
	int GetBit()
	{
		return m_Bit;
	}
	
	void CreateParticle()
	{
		int boneIdx = m_Player.GetBoneIndexByName(m_Bone);
		m_BleedingEffect = new BleedingSourceEffect();
		SEffectManager.PlayInWorld( m_BleedingEffect, "0 0 0" );
		m_BloodParticle = m_BleedingEffect.GetParticle();
		m_BloodParticle.SetOrientation(m_Orientation);
		vector pos;
		pos += m_Offset;
		m_BloodParticle.SetPosition(pos);
		float time = Math.RandomFloat01() * 2;
		//time = time;
		m_BloodParticle.SetParameter(-1, EmitorParam.CURRENT_TIME, time);
		//m_BloodParticle.SetParameter(1, EmitorParam.CURRENT_TIME, time);
		
		m_Player.AddChild(m_BloodParticle, boneIdx);
	}
	
	void RemoveParticle()
	{
		if(m_BleedingEffect) delete m_BleedingEffect;
	}

	void OnUpdateServer(float deltatime, bool no_blood_loss)
	{
		if( !no_blood_loss )
		{
			m_Player.AddHealth("GlobalHealth","Blood", (PlayerConstants.BLEEDING_SOURCE_BLOODLOSS_PER_SEC * deltatime) );
		}
		
	}
	
	void DrawDebugShape()
	{
		if(m_DebugShape)
		{
			Debug.RemoveShape(m_DebugShape);
		}
		
		if(m_DebugShape1) 
		{
			Debug.RemoveShape(m_DebugShape1);
		}

		Particle p = m_BleedingEffect.GetParticle();
		vector pos = p.GetPosition();
		m_DebugShape = Debug.DrawSphere(pos, 0.009, COLOR_BLUE, ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE|ShapeFlags.NOZBUFFER);
		vector arrow_to = m_BloodParticle.GetOrientation();
		arrow_to = arrow_to.AnglesToVector();
		arrow_to = -arrow_to * 0.3;
		arrow_to = pos + arrow_to;
		
		m_DebugShape1 = Debug.DrawArrow(pos, arrow_to, 0.1, COLOR_GREEN);
	}
}