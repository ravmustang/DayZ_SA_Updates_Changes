class LargeTent extends TentBase
{
	void LargeTent()
	{		
		m_ToggleAnimations.Insert( new ToggleAnimations("EntranceO", "EntranceC"), 0 );
		m_ToggleAnimations.Insert( new ToggleAnimations("Window1O", "Window1C"), 0 );
		m_ToggleAnimations.Insert( new ToggleAnimations("Window2O", "Window2C"), 0 );
		m_ToggleAnimations.Insert( new ToggleAnimations("Window3O", "Window3C"), 0 );
		m_ToggleAnimations.Insert( new ToggleAnimations("Window4O", "Window4C"), 0 );
		m_ToggleAnimations.Insert( new ToggleAnimations("Window5O", "Window5C"), 0 );
		m_ToggleAnimations.Insert( new ToggleAnimations("Window6O", "Window6C"), 0 );
		m_ToggleAnimations.Insert( new ToggleAnimations("Window7O", "Window7C"), 0 );
			
		m_ShowAnimationsWhenPitched.Insert( "Body" );
		m_ShowAnimationsWhenPitched.Insert( "EntranceO" );
		m_ShowAnimationsWhenPitched.Insert( "Window1O" );
		m_ShowAnimationsWhenPitched.Insert( "Window2O" );
		m_ShowAnimationsWhenPitched.Insert( "Window3O" );
		m_ShowAnimationsWhenPitched.Insert( "Window4O" );
		m_ShowAnimationsWhenPitched.Insert( "Window5O" );
		m_ShowAnimationsWhenPitched.Insert( "Window6O" );
		m_ShowAnimationsWhenPitched.Insert( "Window7O" );
		
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
		return "largetent_deploy_SoundSet";
	}	
	
	override bool IsTwoHandedBehaviour()
	{
		return true;
	}
	
	override string GetSoundOpen()
	{
		return "LargeTent_Door_Open_SoundSet";
	}
	
	override string GetSoundClose()
	{
		return "LargeTent_Door_Close_SoundSet";
	}
	
	override string GetSoundOpenWindow()
	{
		return "LargeTent_Window_Open_SoundSet";
	}
	
	override string GetSoundCloseWindow()
	{
		return "LargeTent_Window_Close_SoundSet";
	}
};
