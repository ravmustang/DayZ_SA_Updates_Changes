//TODO trees are static objects, there is no script event for playing sounds on clients when they are chopped down.
class ToolBase extends ItemBase
{	
	void ToolBase()
	{
		
	}
			
	override void OnRPC(PlayerIdentity sender, int rpc_type,ParamsReadContext  ctx) 
	{
		super.OnRPC(sender, rpc_type,ctx);
		
		switch(rpc_type)
		{
			case PlantType.TREE_HARD:
				SoundHardTreeFallingPlay();
			break;
			
			case PlantType.TREE_SOFT:
				SoundSoftTreeFallingPlay();
			break;
			
			case PlantType.BUSH_HARD:
				SoundHardBushFallingPlay();
			break;
			
			case PlantType.BUSH_SOFT:
				SoundSoftBushFallingPlay();
			break;
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
		
	void SoundSoftBushFallingPlay()
	{
		EffectSound sound =	SEffectManager.PlaySound( "softBushFall_SoundSet", GetPosition() );
		sound.SetSoundAutodestroy( true );
	}
}