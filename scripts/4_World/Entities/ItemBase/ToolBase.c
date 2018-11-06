//TODO trees are static objects, there is no script event for playing sounds on clients when they are chopped down. Below is a temporary sollution. Redo once trees are entities
class ToolBase extends ItemBase
{
	//m_SynchSwitch invokes synch everytime a tree is choped
	protected bool m_SynchSwitch;
	protected bool m_HaveCutHardTree;
	protected bool m_HaveCutSoftTree;
	protected bool m_HaveCutHardBush;
	protected bool m_HaveCutSoftBush;
	
	void ToolBase()
	{
		m_SynchSwitch = false;
		m_HaveCutHardTree = false;
		m_HaveCutSoftTree = false;
		m_HaveCutHardBush = false;
		m_HaveCutSoftBush = false;
		
		RegisterNetSyncVariableBool("m_SynchSwitch");
		RegisterNetSyncVariableBool("m_HaveCutHardTree");
		RegisterNetSyncVariableBool("m_HaveCutSoftTree");
		RegisterNetSyncVariableBool("m_HaveCutHardBush");
		RegisterNetSyncVariableBool("m_HaveCutSoftBush");
	}
	
	void HardTreeCut()
	{
		m_SynchSwitch = !m_SynchSwitch;
		m_HaveCutHardTree = true;
		SetSynchDirty();
		
		//Local singleplayer
		if ( !GetGame().IsMultiplayer() )
		{
			SoundHardTreeFallingPlay();
		}
	}
	
	void SoftTreeCut()
	{
		m_SynchSwitch = !m_SynchSwitch;
		m_HaveCutSoftTree = true;
		SetSynchDirty();
		
		//Local singleplayer
		if ( !GetGame().IsMultiplayer() )
		{
			SoundSoftTreeFallingPlay();
		}
	}
	
	void HardBushCut()
	{
		m_SynchSwitch = !m_SynchSwitch;
		m_HaveCutHardBush = true;
		SetSynchDirty();
		
		//Local singleplayer
		if ( !GetGame().IsMultiplayer() )
		{
			SoundHardBushFallingPlay();
		}
	}
	
	void SoftBushCut()
	{
		m_SynchSwitch = !m_SynchSwitch;
		m_HaveCutSoftBush = true;
		SetSynchDirty();
		
		//Local singleplayer
		if ( !GetGame().IsMultiplayer() )
		{
			SoundSoftBushFalling();
		}
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
							
		if ( GetGame().IsMultiplayer() )
		{
			if ( m_HaveCutHardTree )
			{
				SoundHardTreeFallingPlay();
			}
			
			if ( m_HaveCutSoftTree )
			{
				SoundSoftTreeFallingPlay();
			}
			
			if ( m_HaveCutHardBush )
			{
				SoundHardBushFallingPlay();
			}
			
			if ( m_HaveCutSoftBush )
			{
				SoundSoftBushFalling();
			}
		}
	}
	
	void SoundHardTreeFallingPlay()
	{
		EffectSound sound =	SEffectManager.PlaySound( "hardTreeFall_SoundSet", GetPosition() );
		sound.SetSoundAutodestroy( true );
	}
		
	void SoundSoftTreeFallingPlay()
	{
		EffectSound sound =	SEffectManager.PlaySound( "softTreeFall_SoundSet", GetPosition() );
		sound.SetSoundAutodestroy( true );
	}
		
	void SoundHardBushFallingPlay()
	{
		EffectSound sound =	SEffectManager.PlaySound( "hardBushFall_SoundSet", GetPosition() );
		sound.SetSoundAutodestroy( true );
	}
		
	void SoundSoftBushFalling()
	{
		EffectSound sound =	SEffectManager.PlaySound( "softBushFall_SoundSet", GetPosition() );
		sound.SetSoundAutodestroy( true );
	}
	
	//Reset synchronization after the UA is completed.
	override void SoundSynchRemoteReset()
	{
		super.SoundSynchRemoteReset();
		
		m_HaveCutHardTree = false;
		m_HaveCutSoftTree = false;
		m_HaveCutHardBush = false;
		m_HaveCutSoftBush = false;
	}
	
	bool GetHaveCutHardTree()
	{
		return m_HaveCutHardTree;
	}
	
	bool GetHaveCutSoftTree()
	{
		return m_HaveCutSoftTree;
	}
	
	bool GetHaveCutHardBush()
	{
		return m_HaveCutHardBush;
	}
	
	bool GetHaveCutSoftBush()
	{
		return m_HaveCutSoftBush;
	}
}