class Effect : Managed
{
	protected int				m_ID;
    protected vector    		m_Position;
	protected bool				m_IsPlaying;	
	
	// Attachment params
	protected Object 			m_ParentObject;
	protected vector    		m_LocalPos;
	protected vector    		m_LocalOri;
	
	
	//=====================================
	// Constructor
	//=====================================
	void Effect()
	{
		
	}
	
	//=====================================
	// Destructor
	//=====================================
	void ~Effect()
	{
		
	}
	
	//=====================================
	// IsSound
	//=====================================
	bool IsSound()
	{
		return false;
	}
	
	//=====================================
	// SetID
	//=====================================
	void SetID(int id)
	{
		m_ID = id;
	}
	
	//=====================================
	// GetID
	//=====================================
	int GetID()
	{
		return m_ID;
	}
	
	//=====================================
	// GetAttachmentParent
	//=====================================
	Object GetAttachmentParent()
	{
		return m_ParentObject;
	}
	
	//=====================================
	// GetAttachedLocalPos
	//=====================================
	vector GetAttachedLocalPos()
	{
		return m_LocalPos;
	}
	
	//=====================================
	// GetAttachedLocalOri
	//=====================================
	vector GetAttachedLocalOri()
	{
		return m_LocalOri;
	}
	
	//=====================================
	// SetAttachmentParent
	//=====================================
	void SetAttachmentParent(Object obj)
	{
		m_ParentObject = obj;
	}
	
	//=====================================
	// SetAttachedLocalPos
	//=====================================
	void SetAttachedLocalPos(vector pos)
	{
		m_LocalPos = pos;
	}
	
	//=====================================
	// SetAttachedLocalOri
	//=====================================
	void SetAttachedLocalOri(vector ori)
	{
		m_LocalOri = ori;
	}
		
	// ! Returns true if the effect is being played right now. False if otherwise.
	//=====================================
	// IsPlaying
	//=====================================
	bool IsPlaying()
	{
		return m_IsPlaying;
	}
	
	//=====================================
	// SetPosition
	//=====================================
	void SetPosition( vector pos )
    {
        m_Position = pos;
    }

	//=====================================
	// GetPosition
	//=====================================
	vector GetPosition()
	{
		return m_Position;
	}

	
	// !Plays all elements this effects consists of
	//=====================================
	// Play
	//=====================================
    void Start()
    {		
		m_IsPlaying = true;
		
		Event_OnStarted();
	}
	
	//! Stops all elements this effect consists of. Alternatively use SEffectManager.Stop( effect_id )
	//=====================================
	// Stop
	//=====================================
    void Stop()
    {
		m_IsPlaying = false;
    }
	
	//=====================================
	// SetEnableEventFrame
	//=====================================
	void SetEnableEventFrame(bool enable)
	{
		if ( enable )
		{
			SEffectManager.Event_OnFrameUpdate.Insert(this.Event_OnFrameUpdate);			
		}
		else
		{
			SEffectManager.Event_OnFrameUpdate.Remove(this.Event_OnFrameUpdate);
		}
	}
	
	//=====================================
	// Event_OnFrameUpdate
	//=====================================
	void Event_OnFrameUpdate(float time_delta)
	{
		// Override this method for own use
	}
	
	//=====================================
	// Event_OnPlayStarted
	//=====================================
	void Event_OnStarted()
	{
		// Override this method for own use
	}
	
	//=====================================
	// OnCheckUpdate
	//=====================================
	void OnCheckUpdate()
	{
		
	}
}