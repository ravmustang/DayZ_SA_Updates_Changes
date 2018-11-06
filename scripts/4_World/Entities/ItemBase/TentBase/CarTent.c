class CarTent extends TentBase
{
	void CarTent()
	{		
		m_ToggleAnimations.Insert( new ToggleAnimations("EntranceO", "EntranceC"), 0 );
			
		m_ShowAnimationsWhenPitched.Insert( "Body" );
		m_ShowAnimationsWhenPitched.Insert( "EntranceO" );

		m_ShowAnimationsWhenPacked.Insert( "Inventory" );
	}
		
	override void EEInit()
	{		
		super.EEInit();
	}
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{		
		super.OnItemLocationChanged(old_owner, new_owner);
	}
	
	override void OnPlacementComplete( Man player )
	{		
		super.OnPlacementComplete( player );

		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			SoundParams soundParams = new SoundParams("placeCarTent_SoundSet");
			SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
			SoundObject soundObject = soundBuilder.BuildSoundObject();
			soundObject.SetPosition(GetPosition());
			GetGame().GetSoundScene().Play3D(soundObject, soundBuilder);
		}		
	}
	
	override string GetDeploySoundset()
	{
		return "cartent_deploy_SoundSet";
	}	
	
	override bool IsHeavyBehaviour()
	{
		return true;
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	
	override string GetSoundOpen()
	{
		return "CarTent_Door_Open_SoundSet";
	}
	
	override string GetSoundClose()
	{
		return "CarTent_Door_Close_SoundSet";
	}
};
