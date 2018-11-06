class MediumTent extends TentBase
{
	void MediumTent()
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

		//temporary used sound for car tent
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			SoundParams soundParams = new SoundParams("placeCarTent_SoundSet");
			SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
			SoundObject soundObject = soundBuilder.BuildSoundObject();
			soundObject.SetPosition(GetPosition());
			GetGame().GetSoundScene().Play3D(soundObject, soundBuilder);
		}	
		
		if ( GetGame().IsServer() )
		{
			m_ClutterCutter = GetGame().CreateObject( "ClutterCutter6x6", GetPosition(), false );	
		}	
	}
	
	override string GetDeploySoundset()
	{
		return "mediumtent_deploy_SoundSet";
	}	
	
	override bool IsTwoHandedBehaviour()
	{
		return true;
	}
	
	override string GetSoundOpen()
	{
		return "MediumTent_Door_Open_SoundSet";
	}
	
	override string GetSoundClose()
	{
		return "MediumTent_Door_Close_SoundSet";
	}
};
