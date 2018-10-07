class ActionIgniteFireplaceByRoadFlare: ActionIgniteFireplace
{	
	void ActionIgniteFireplaceByRoadFlare()
	{
		m_CallbackClass = ActionIgniteFireplaceCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIRETORCH;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting the fireplace with a road flare.";
		m_MessageSuccess = "I have ignited the fireplace with a road flare.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}

	override int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_ROADFLARE;
	}
		
	override string GetText()
	{
		return "Ignite by road flare";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		if ( target_object && target_object.IsFireplace() )
		{
			FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
			
			if ( !fireplace_target.IsBurning()  &&  item.HasEnergyManager()  &&  item.GetCompEM().IsWorking() ) 
			{
				if ( fireplace_target.IsBarrelWithHoles() ) 		//if barrel with holes
				{
					BarrelHoles_ColorBase barrel = BarrelHoles_ColorBase.Cast( fireplace_target );
					if ( !barrel.IsOpened() )
					{
						return false;
					}
				}
				
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		Object target_object = action_data.m_Target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
		
		//remove grass
		Object cc_object = GetGame().CreateObject ( fireplace_target.OBJECT_CLUTTER_CUTTER , target_object.GetPosition() );
		cc_object.SetOrientation ( target_object.GetOrientation() );
		DestroyClutterCutterAfterTime();
		
		//start fire
		fireplace_target.StartFire();

		//add specialty
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}