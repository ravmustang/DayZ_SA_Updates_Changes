//WIP

class Effect
{
	protected int       		m_ParticleID = -1;
    protected string    		m_SoundName;
    protected vector    		m_Position;
    protected vector    		m_Orientation;
	protected Particle 			m_Particle;
	protected SoundOnVehicle 	m_Sound;
	protected int				m_ID;
	
	void ~Effect()
	{
		SEffectManager.EffectUnregister( GetID() );
		Stop();
	}
	
	void SetID(int id)
	{
		m_ID = id;
	}
	
	int GetID()
	{
		return m_ID;
	}
	
	void AttachTo(Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
	{
		GetParticle().AddAsChild(obj, local_pos, local_ori);
	}
	
	//! Set orientation in degrees (yaw, pitch, roll)
	void SetOrientation( vector ori )
	{
		m_Orientation = ori;
	}
	
	vector GetOrientation()
	{
		return m_Orientation;
	}
	
	Particle GetParticle()
	{
		return m_Particle;
	}
	
	void SetParticle(Particle p)
	{
		m_Particle = p;
	}
	
	SoundOnVehicle GetSound()
	{
		return m_Sound;
	}
	
	void GetLight()
	{
		return; // TO DO!
	}
	
	void GetDecal()
	{
		return; // TO DO!
	}
	
	// !Plays all elements this effects consists of
    void Play()
    {
		if (m_ParticleID > 0)
		{
			Particle p;
			p = Particle.Play(m_ParticleID, GetPosition());
			
			p.SetOrientation( GetOrientation() );
			p.SetEffectHolder(this);
			
			SetParticle(p);
		}
		
		if (m_SoundName != "")
		{
			//PlaySoundInWorld(m_SoundName, 50); // TO DO: SUCH FUNCTION DOESN'T EXIST! SOMEONE NEEDS TO CREATE ENGINE SUPPORT FOR IT!
			
			// TEMPORAL HACK: Because we can't play sounds at world coordinates, we play them on player.
			if (!GetGame().IsMultiplayer())
			{
				Object plr = GetGame().GetPlayer();
				GetGame().CreateSoundOnObject(plr, m_SoundName, 50, false, true);
			}
		}
		
		OnPlay();
	}
	
	// Override this method for own use
	void OnPlay()
	{
		
	}
	
	void CheckLifeSpan()
	{
		if (!m_Particle  &&  !m_Sound)
		{
			//Print(this);
			delete this;
		}
	}
 
    void SetPosition( vector pos )
    {
        m_Position = pos;
    }
	
	void SetParticleID( int id )
    {
        m_ParticleID = id;
    }
	
	void SetSoundName(string snd)
	{
		m_SoundName = snd;
	}
	
	vector GetPosition()
	{
		return m_Position;
	}
	
	//! Stops all elements this effect consists of. Alternatively use SEffectManager.Stop( effect_id )
    void Stop()
    {
		if (GetParticle())
		{
			GetParticle().Stop();
			m_Particle = NULL;
		}
		
		if (GetSound())
		{
			GetGame().ObjectDelete(GetSound());
			m_Sound = NULL;
		}
    }
}